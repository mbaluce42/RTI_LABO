#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>
#include <string>

using namespace std;

/*fait un appel à socket() pour créer la socket
o construit l’adresse réseau de la socket par appel à getaddrinfo()
o fait appel à bind() pour lier la socket à l’adresse réseau*/
int ServerSocket(char *port)
{
    int sServeur;
    struct addrinfo hints, *res;
    int status;
    int yes = 1;

    //Initialisation de la structure hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_STREAM; //TCP (mode connecté)
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE; //Pour remplir automatiquement l'adresse IP
    

    //Récupération des informations sur le serveur
    if ((status = getaddrinfo(NULL, port, &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }

    //Création de la socket
    if ((sServeur = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
    {
        perror("Erreur de socket(): ");
        return -1;
    }

    //Permet de réutiliser le port
    if (setsockopt(sServeur, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt");
        return -1;
    }

    //Liaison de la socket à l'adresse IP et au port
    if (bind(sServeur, res->ai_addr, res->ai_addrlen) == -1)
    {
        perror("bind");
        return -1;
    }

    //Libération de la structure res (la liste chainée)
    freeaddrinfo(res);

    //Mise en écoute de la socket
    if (listen(sServeur, SOMAXCONN) == -1)
    {
        perror("listen");
        return -1;
    }

    return sServeur;
}

/*fait appel à socket() pour créer la socket
o construit l’adresse réseau de la socket (avec l’IP et le port du serveur) par appel à la fonction getaddrinfo()
o fait appel à connect() pour se connecter sur le serveur*/
int ClientSocket(char *ipServeur, char *portServeur)
{
    int sClient;
    struct addrinfo hints, *res;
    int status;

    //Initialisation de la structure hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_STREAM; //TCP (mode connecté)
    hints.ai_protocol = IPPROTO_TCP;

    //Récupération des informations sur le serveur
    if ((status = getaddrinfo(ipServeur, portServeur, &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }

    //Création de la socket
    if ((sClient = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
    {
        perror("Erreur de socket(): ");
        return -1;
    }

    //Connexion au serveur
    if (connect(sClient, res->ai_addr, res->ai_addrlen) == -1)
    {
        perror("connect");
        return -1;
    }

    //Libération de la structure res (la liste chainée)
    freeaddrinfo(res);

    return sClient;

}

int AcceptSocket(int sEcoute, char *ipClient)
{
    int sClient;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    char ip[INET6_ADDRSTRLEN];

    addr_size = sizeof their_addr;

    //Acceptation de la connexion entrante
    sClient = accept(sEcoute, (struct sockaddr *)&their_addr, &addr_size);
    if (sClient == -1)
    {
        perror("accept");
        return -1;
    }

    //Récupération de l'adresse IP du client
    if (ipClient != NULL)
    {
        if (their_addr.ss_family == AF_INET)
        {
            struct sockaddr_in *s = (struct sockaddr_in *)&their_addr;
            inet_ntop(AF_INET, &s->sin_addr, ip, sizeof ip);
        }
        else
        {
            struct sockaddr_in6 *s = (struct sockaddr_in6 *)&their_addr;
            inet_ntop(AF_INET6, &s->sin6_addr, ip, sizeof ip);
        }
        strcpy(ipClient, ip);
    }

    return sClient;

}


int SendSocket(int sSocket, char *buffer,int taille)
{
    int nbOctets;

    //Envoi du message
    if ((nbOctets = send(sSocket, buffer, taille, 0)) == -1)
    {
        perror("send");
        return -1;
    }

    return nbOctets;

}


int ReceiveSocket(int socket, char *buffer)
{
    int nbOctets;

    //Réception du message
    if ((nbOctets = recv(socket, buffer, TAILLE_BUFFER, 0)) == -1)
    {
        perror("recv");
        return -1;
    }

    return nbOctets;
}


int SendSocket(int sSocket, string &buffer)
{
    int nbOctets = 0;
    size_t totalEnvoyes = 0;
    size_t tailleMessage = buffer.size();

    // Boucle pour s'assurer que tous les octets sont envoyés
    while (totalEnvoyes < tailleMessage) {
        nbOctets = send(sSocket, buffer.c_str() + totalEnvoyes, tailleMessage - totalEnvoyes, 0);
        if (nbOctets == -1) {
            perror("Erreur lors de l'envoi (send)");
            return -1;
        }
        totalEnvoyes += nbOctets;
    }

    return totalEnvoyes;
}

int ReceiveSocket(int socket, string &buffer)
{
    char bufferChar[TAILLE_BUFFER];
    int nbOctets;

    // Réinitialiser le buffer avant de recevoir des données
    memset(bufferChar, 0, TAILLE_BUFFER);

    // Réception du message
    nbOctets = recv(socket, bufferChar, TAILLE_BUFFER - 1, 0); // Laisser de la place pour '\0'
    if (nbOctets == -1) {
        perror("Erreur lors de la réception (recv)");
        return -1;
    } else if (nbOctets == 0) {
        // Le client a fermé la connexion
        printf("Le client a fermé la connexion\n");
        return 0;
    }

    // Convertir en string le buffer reçu
    buffer.assign(bufferChar, nbOctets);  // Convertir uniquement les octets reçus

    return nbOctets;
}

