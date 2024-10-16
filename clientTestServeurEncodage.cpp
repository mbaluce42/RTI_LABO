#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "./Librairies/socket.h"
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

int sClient;
void HandlerSIGINT(int s);
int Echange(char *requete, char *reponse);
int Echange(string requete, string& reponse);
bool OBEP_Login(const char *user, const char *password);
void OBEP_Logout();
void SMOP_Operation(char op, int a, int b);
int OBEP_Op(char* requete, string& resultat);
int OBEP_Op(string requete, string& resultat);
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Erreur...\n");
        printf("USAGE : Client ipServeur portServeur\n");
        exit(1);
    }
    // Armement des signaux
    struct sigaction A;
    A.sa_flags = 0;
    sigemptyset(&A.sa_mask);
    A.sa_handler = HandlerSIGINT;
    if (sigaction(SIGINT, &A, NULL) == -1)
    {
        perror("Erreur de sigaction");
        exit(1);
    }
    // Connexion sur le serveur
    if ((sClient = ClientSocket(&*argv[1], &*argv[2])) == -1)
    {
        perror("Erreur de ClientSocket");
        exit(1);
    }
    printf("Connecte sur le serveur.\n");

    printf("Bienvenu sur le client Test du Serveur Encodage\n");
    printf("Toutes les operations sont a taper sur la ligne de commande\n");
    //print menu
    printf("Liste des operations disponibles :\n\n");
    
    printf("1.  GET_SUBJECTS#\n");
    printf("2.  GETID_SUBJECT#[nomSujet]\n");
    printf("3.  GET_AUTHORS#\n");
    printf("4.  GETID_AUTHOR#[prenomAuteur] [nomAuteur]\n");
    printf("5.  GET_BOOKS#\n");
    printf("6.  GETID_BOOK#[titreLivre]\n");
    printf("7.  GETID_EMPLOYEE#[loginEmploye]\n");
    printf("8.  GET_ENCODED_BOOKS#\n");
    printf("9. GET_ENCODEDBOOKSBYEMPLOYEE#[idEmploye]\n");

    printf("10.  GET_EMPLOYEES#\n\n");
    printf("11. ADD_AUTHOR#[nomAuteur]#[prenomAuteur]#[dateNaissance]\n");
    printf("12. ADD_SUBJECT#[nomSujet]\n");
    printf("13. ADD_BOOK#[idAuteur]#[idSujet]#[titre]#[isbn]#[nombrePages]#[stockDisponible]#[prix]#[anneePublication]\n");
    printf("14. ADD_EMPLOYEE#[login]#[motDePasse]\n");
    printf("15. ADD_ENCODED_BOOK#[idEmploye]#[idLivre]#[date]\n\n");


    //remplacer [] par les valeurs
    //exemple: ADD_AUTHOR#Doe#John#01/01/2000
    printf("remplacer [] par les valeurs\n");
    printf("format de la date: aaa-mm-jj\n");
    printf("format de l'isbn: 978-2123456803(3 chiffres avant le tiret et 10 chiffres apres)\n\n");    


    while (1)
    {
        printf("Operation (<CTRL-C> four fin) : ");
        /*string operation;
        getline(cin, operation, '\n');

        int res = OBEP_Op(operation, operation);
        if (res == 0)
        {
            printf("Reponse: %s\n", operation.c_str());
        }
        else
        {
            printf("Erreur: %s\n", operation.c_str());
        }*/

        char requete[4048];
        string reponse;
        printf("Operation (<CTRL-C> four fin) : ");
        fflush(stdin);
        //scanf("%d %c %d",&a,&op,&b); // pas ouf !
        fgets(requete, 4048, stdin);
        //obep char
        int res=OBEP_Op(requete,reponse);
        if(res==0)
        {
            printf("Reponse: %s\n", reponse.c_str());
        }
        else
        {
            printf("Erreur: %s\n", reponse.c_str());
        }
    }
}
/*
//***** Fin de connexion ********************************************
void HandlerSIGINT(int s)
{
    printf("\nArret du client.\n");
    SMOP_Logout();
    close(sClient);
    exit(0);
}
//***** Gestion du protocole SMOP ***********************************
bool SMOP_Login(const char* user,const char* password)
{
    char requete[200], reponse[200];
    bool onContinue = true;
    // ***** Construction de la requete ********************* 
    sprintf(requete,"LOGIN#%s#%s",user,password);
    // ***** Envoi requete + réception réponse ************** 
    Echange(requete,reponse);
    // ***** Parsing de la réponse ************************** 
    char *ptr = strtok(reponse,"#"); // entête = LOGIN (normalement...) 
    ptr = strtok(NULL,"#"); // statut = ok ou ko
    if (strcmp(ptr, "ok") == 0 || strcmp(ptr, "OK") == 0)
        printf("Login OK.\n");
    else
    {
        ptr = strtok(NULL, "#"); // raison du ko
        printf("Erreur de login: %s\n", ptr);
        onContinue = false;
    }
    return onContinue;
}
//******************************************************************* 
void SMOP_Logout()
{
    char requete[200], reponse[200];
    int nbEcrits, nbLus;
    // ***** Construction de la requete ********************* 
    sprintf(requete,"LOGOUT#");
    // ***** Envoi requete + réception réponse ************** 
    Echange(requete,reponse);
    // ***** Parsing de la réponse **************************
    // pas vraiment utile...
}
//*******************************************************************
void SMOP_Operation(char op,int a,int b)
{
    char requete[200], reponse[200];
    // ***** Construction de la requete *********************
    sprintf(requete, "OPER#%c#%d#%d", op, a, b);
    // ***** Envoi requete + réception réponse ************** 
    Echange(requete,reponse);
    // ***** Parsing de la réponse ************************** 
    char *ptr = strtok(reponse,"#"); // entête = OPER (normalement...) 
    ptr = strtok(NULL,"#"); // statut = ok ou ko
    if (strcmp(ptr, "ok") == 0 || strcmp(ptr, "OK") == 0)
    {
        ptr = strtok(NULL, "#"); // résultat du calcul
        printf("Résultat = %s\n", ptr);
    }
    else
    {
        ptr = strtok(NULL, "#"); // raison du ko
        printf("Erreur: %s\n", ptr);
    }
}
//*******************************************************************
void SMOP_Op(char* requete)
{
    char reponse[4048];
    // *****Envoi requete + réception réponse **************
    Echange(requete, reponse);
    // *****Parsing de la réponse **************************
    istringstream is(reponse);
    string TypeRequete;
    getline(is, TypeRequete, '_'); // skip le type (add, get)
    if(TypeRequete == "ADD")
    {
        getline(is, TypeRequete, '#');//skip le type (Author, Subject, Book)

        string status;
        string message;
        getline(is, status, '#');//recupere le status de la requete
        getline(is, message, '\n'); //recupere le message de la requete

        if(status == "OK")
        {
            printf("Pas d'erreur: %s\n", message.c_str());
        }
        else
        {
            printf("Erreur: %s\n", message.c_str());
        }

    }
    else if(TypeRequete == "GET")
    {
        getline(is, TypeRequete, '#');//skip le type (Author, Subject, Book)
        string status;
        string messages;
        getline(is, status, '\n');//recupere le status de la requete
        getline(is, messages, '\n'); //recupere le message de la requete
        if(status == "OK")
        {
            printf("Pas d'erreur: \n" );
            printf("%s\n", messages.c_str());
            while (getline(is, messages, '\n'))
            {
                printf("%s\n", messages.c_str());
            }
            
        }
        else
        {
            printf("Erreur: %s\n", messages.c_str());
        }

    }

}
//***** Echange de données entre client et serveur ******************
void Echange(char *requete, char *reponse)
{
    int nbEcrits, nbLus;
    // ***** Envoi de la requete ****************************
    if ((nbEcrits = SendSocket(sClient, requete, strlen(requete))) == -1)
    {
        perror("Erreur de Send");
        close(sClient);
        exit(1);
    }
    // ***** Attente de la reponse **************************
    if ((nbLus = ReceiveSocket(sClient, reponse)) < 0)
    {
        perror("Erreur de Receive");
        close(sClient);
        exit(1);
    }
    if (nbLus == 0)
    {
        printf("Serveur arrete, pas de reponse reçue...\n");
        close(sClient);
        exit(1);
    }
    reponse[nbLus] = 0;
}

*/

void HandlerSIGINT(int s) 
{
    printf("\nArret du client.\n");
    //OBEP_Logout();
    close(sClient);
    exit(0);
}

//***** Gestion du protocole SMOP ***********************************
bool OBEP_Login(const char* user,const char* password)
{
    char requete[200], reponse[200];
    bool onContinue = true;
    // ***** Construction de la requete ********************* 
    sprintf(requete,"LOGIN#%s#%s",user,password);
    // ***** Envoi requete + réception réponse ************** 
    Echange(requete,reponse);
    // ***** Parsing de la réponse ************************** 
    char *ptr = strtok(reponse,"#"); // entête = LOGIN (normalement...) 
    ptr = strtok(NULL,"#"); // statut = ok ou ko
    if (strcmp(ptr, "ok") == 0 || strcmp(ptr, "OK") == 0)
        printf("Login OK.\n");
    else
    {
        ptr = strtok(NULL, "#"); // raison du ko
        printf("Erreur de login: %s\n", ptr);
        onContinue = false;
    }
    return onContinue;
}
//******************************************************************* 
void OBEP_Logout()
{
    char requete[200], reponse[200];
    int nbEcrits, nbLus;
    // ***** Construction de la requete ********************* 
    sprintf(requete,"LOGOUT");
    // ***** Envoi requete + réception réponse ************** 
    Echange(requete,reponse);
    // ***** Parsing de la réponse **************************
    // pas vraiment utile...
}

//*******************************************************************
//en string
int OBEP_Op(string requete, string& resultat)
{
    //char reponse[4048];
    string reponse;
    int res;
    // *****Envoi requete + réception réponse **************
    res=Echange(requete, reponse);
    if(res<0)
    {
        return -1;
    }
    else
    {
        // *****Parsing de la réponse **************************
        istringstream is(reponse);
        string TypeRequete;
        getline(is, TypeRequete, '_'); // skip le type (add, get)
        if(TypeRequete == "ADD")
        {
            getline(is, TypeRequete, '#');//skip le type (Author, Subject, Book)

            string status;
            string message;
            getline(is, status, '#');//recupere le status de la requete
            getline(is, message, '\n'); //recupere le message de la requete
            resultat = message;
            if(status == "OK")
            {
                
            }
            else
            {
                return -1;
            }
        }
        else if(TypeRequete == "GET")
        {
            getline(is, TypeRequete, '#');//skip le type (Author, Subject, Book)
            string status;
            string messages;
            getline(is, status, '\n');//recupere le status de la requete
            getline(is, messages, '\n'); //recupere le message de la requete
            if(status == "OK")
            {
                resultat = messages + '\n';
                while (getline(is, messages, '\n'))
                {
                    resultat += messages;
                    resultat += '\n';
                }
                
            }
            else
            {
                
                istringstream is(status);
                getline(is, status, '#');//ignore le status

                getline(is, messages, '\n'); //recupere le message de la requete
                resultat = messages;
                return -1;
            }
        }
        else if(TypeRequete=="GETID")
        {
            getline(is, TypeRequete, '#');//skip le type (Author, Subject)
            string status;
            string messages;
            getline(is, status, '\n');//recupere le status de la requete
            getline(is, messages, '\n'); //recupere le message de la requete
            if(status == "OK")
            {
                resultat = messages;
            }
            else
            {
                istringstream is(status);
                getline(is, status, '#');//ignore le status

                getline(is, messages, '\n'); //recupere le message de la requete
                resultat = messages;
                return -1;
            }
        }
        else
        {
            istringstream is(reponse);
            string TypeRequete;
            getline(is, TypeRequete, '#'); // skip le type (Author, Subject, Book)
            
            if(TypeRequete == "LOGIN")
            {
                string status;
                getline(is, status, '#');
                string message;
                getline(is, message, '\n');
                resultat=message;
                if(status == "OK")
                {
                
                }
                else
                {
                    return -1;
                }
            }
            else if(TypeRequete == "LOGOUT")
            {
                string status;
                getline(is, status, '#');
                string message;
                getline(is, message, '\n');
                resultat=message;
                if(status == "OK")
                {
                
                }
                else
                {
                    return -1;
                }
            }
            else //erreur
            {
                string status;
                getline(is, status, '#');
                string message;
                getline(is, message, '\n');
                resultat=message;
                return -1;
            }
        }

    }

    printf("Reponse OBEP(return 0): %s\n", resultat.c_str());

    return 0;
}
//en char
int OBEP_Op(char* requete, string& resultat)
{
    char reponse[4048];
    int res;
    // *****Envoi requete + réception réponse **************
    res=Echange(requete, reponse);
    if(res<0)
    {
        return -1;
    }
    else
    {
        // *****Parsing de la réponse **************************
        istringstream is(reponse);
        string TypeRequete;
        getline(is, TypeRequete, '_'); // skip le type (add, get)
        if(TypeRequete == "ADD")
        {
            getline(is, TypeRequete, '#');//skip le type (Author, Subject, Book)

            string status;
            string message;
            getline(is, status, '#');//recupere le status de la requete
            getline(is, message, '\n'); //recupere le message de la requete
            resultat = message;
            if(status == "OK")
            {
                
            }
            else
            {
                return -1;
            }
        }
        else if(TypeRequete == "GET")
        {
            getline(is, TypeRequete, '#');//skip le type (Author, Subject, Book)
            string status;
            string messages;
            getline(is, status, '\n');//recupere le status de la requete
            getline(is, messages, '\n'); //recupere le message de la requete
            if(status == "OK")
            {
                resultat = messages + '\n';
                while (getline(is, messages, '\n'))
                {
                    resultat += messages;
                    resultat += '\n';
                }
                
            }
            else
            {
                istringstream is(status);
                getline(is, status, '#');//ignore le status

                getline(is, messages, '\n'); //recupere le message de la requete
                resultat = messages;
                return -1;
            }
        }
        else if(TypeRequete=="GETID")
        {
            getline(is, TypeRequete, '#');//skip le type (Author, Subject)
            string status;
            string messages;
            getline(is, status, '\n');//recupere le status de la requete
            getline(is, messages, '\n'); //recupere le message de la requete
            if(status == "OK")
            {
                resultat = messages;
            }
            else
            {
                istringstream is(status);
                getline(is, status, '#');//ignore le status

                getline(is, messages, '\n'); //recupere le message de la requete
                resultat = messages;
                return -1;
            }
        }

    }

    printf("Reponse OBEP(return 0): %s\n", resultat.c_str());

    return 0;
}

//***** Echange de données entre client et serveur ******************
int Echange(char *requete, char *reponse)
{
    int nbEcrits, nbLus;
    // ***** Envoi de la requete ****************************
    if ((nbEcrits = SendSocket(sClient, requete, strlen(requete))) == -1)
    {
        perror("Erreur de Send");
        close(sClient);
        return -1;
    }
    // ***** Attente de la reponse **************************
    if ((nbLus = ReceiveSocket(sClient, reponse)) < 0)
    {
        perror("Erreur de Receive");
        close(sClient);
        return -2;
    }
    if (nbLus == 0)
    {
        printf("Serveur arrete, pas de reponse reçue...\n");
        close(sClient);
        return -3;
    }
    reponse[nbLus] = 0;

    return 0;
}


int Echange(string requete, string& reponse)
{

    int nbEcrits, nbLus;
    // ***** Envoi de la requete ****************************
    if ((nbEcrits = SendSocket(sClient, requete)) == -1)
    {
        perror("Erreur de Send");
        close(sClient);
        return -1;
    }
    // ***** Attente de la reponse **************************
    if ((nbLus = ReceiveSocket(sClient, reponse)) < 0)
    {
        perror("Erreur de Receive");
        close(sClient);
        return -2;
    }
    if (nbLus == 0)
    {
        printf("Serveur arrete, pas de reponse reçue...\n");
        close(sClient);
        return -3;
    }

    return 0;
}
