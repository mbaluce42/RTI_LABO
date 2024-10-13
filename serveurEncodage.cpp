#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include "./Librairies/socket.h"
#include "./Protocole/Prot_OBEP.h"

#define MAX_THREADS 100
#define CONFIG_FILE "server_config.txt"

int sEcoute;
int PORT_ENCODING;
int NB_THREADS;

pthread_t threads[MAX_THREADS];
pthread_mutex_t mutex_pool = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_pool = PTHREAD_COND_INITIALIZER;

int clients_queue[MAX_THREADS];
int queue_size = 0;
int queue_head = 0;
int queue_pos = 0;

void HandlerSIGINT(int s);
void *ThreadPool(void *arg);
void TraitementConnexion(int sService);

void read_config() 
{
    FILE *file = fopen(CONFIG_FILE, "r");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier de configuration");
        //exit(1);
        PORT_ENCODING= 50000;
        NB_THREADS= 2;
        return;
    }

    if (fscanf(file, "PORT_ENCODING=%d\n", &PORT_ENCODING) != 1 ||
        fscanf(file, "NB_THREADS=%d\n", &NB_THREADS) != 1) {
        fprintf(stderr, "Erreur de lecture du fichier de configuration\n");
        exit(1);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    read_config();

    // Armement des signaux
    struct sigaction A;
    A.sa_flags = 0;
    sigemptyset(&A.sa_mask);
    A.sa_handler = HandlerSIGINT;
    if (sigaction(SIGINT, &A, NULL) == -1) {
        perror("Erreur de sigaction");
        exit(1);
    }

    // Creation de la socket d'écoute
    char port_str[10];
    sprintf(port_str, "%d", PORT_ENCODING);
    if ((sEcoute = ServerSocket(port_str)) == -1) {
        perror("Erreur de ServeurSocket");
        exit(1);
    }

    // Création du pool de threads
    for (int i = 0; i < NB_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, ThreadPool, NULL) != 0) {
            perror("Erreur de création de thread");
            exit(1);
        }
    }

    printf("Demarrage du serveur sur le port %d avec %d threads.\n", PORT_ENCODING, NB_THREADS);

    while (1) {
        printf("Attente d'une connexion...\n");
        char ipClient[50];
        int sService = AcceptSocket(sEcoute, ipClient);
        if (sService == -1) {
            perror("Erreur de Accept");
            continue;
        }
        printf("Connexion acceptée : IP=%s socket=%d\n", ipClient, sService);
        printf("Taille de la queue avant ajout : %d\n", queue_size);
        pthread_mutex_lock(&mutex_pool);
        if (queue_size < NB_THREADS) {
            clients_queue[queue_pos] = sService;
            queue_pos = (queue_pos + 1) % NB_THREADS;
            queue_size++;
            pthread_cond_signal(&cond_pool);
        } else {
            printf("\nFile d'attente pleine, connexion refusée\n");
            close(sService);
        }
        pthread_mutex_unlock(&mutex_pool);
    }
}

void *ThreadPool(void *arg) {
    while (1) {
        int sService;
        pthread_mutex_lock(&mutex_pool);
        while (queue_size == 0) {
            pthread_cond_wait(&cond_pool, &mutex_pool);
        }
        sService = clients_queue[queue_head];
        queue_head = (queue_head + 1) % NB_THREADS;
        queue_size--;
        pthread_mutex_unlock(&mutex_pool);

        printf("\t[THREAD %p] Je m'occupe de la socket %d\n", pthread_self(), sService);
        TraitementConnexion(sService);
    }
    return NULL;
}

void HandlerSIGINT(int s) {
    printf("\nArret du serveur.\n");
    close(sEcoute);
    OBEP_Close();
    exit(0);
}

void TraitementConnexion(int sService) {
    char requete[200], reponse[4048];
    int nbLus, nbEcrits;
    bool onContinue = true;
    while (onContinue) 
    {
        printf("\t[THREAD %p] Attente requete...\n", pthread_self());
        // ***** Reception Requete ******************
        if ((nbLus = ReceiveSocket(sService, requete)) <= 0) 
        {   
            // ***** Fin de connexion ? *****************
            if (nbLus < 0) perror("Erreur de Receive");
            printf("\t[THREAD %p] Fin de connexion du client averc la socket %d\n", pthread_self(), sService);
            close(sService);
            return;
        }
        requete[nbLus] = 0;
        printf("\t[THREAD %p] Requete recue = %s\n", pthread_self(), requete);
        // ***** Traitement de la requete ***********
        onContinue = OBEP_Parser(requete, reponse, sService);
        // ***** Envoi de la reponse ****************
        if ((nbEcrits = SendSocket(sService, reponse, strlen(reponse))) < 0) {
            perror("Erreur de Send");
            close(sService);
            return;
        }
        printf("\t[THREAD %p] Reponse envoyee = %s\n", pthread_self(), reponse);

        //if (!onContinue)
        //    printf("\t[THREAD %p] Fin de connexion de la socket %d\n", pthread_self(), sService);
    }
    close(sService);
}