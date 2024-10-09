#include "Prot_OBEP.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include "../Librairies/socket.h"
#include <unistd.h>

//#include "../ClientBookEncoderQt/FichierClient.h"

using namespace std;


int clientsConnectes[NB_MAX_CLIENTS];
static int nbClientsConnectes=0;

int estPresent(int socketClient)
{
    for(int i=0; i<nbClientsConnectes; i++)
    {
        if(clientsConnectes[i]==socketClient)
        {
            return i; //client deja present
        }
    }
    return -1;//client non present

}
int ajouterClient(int socketClient)
{
    if(nbClientsConnectes<NB_MAX_CLIENTS)
    {
        clientsConnectes[nbClientsConnectes]=socketClient;
        nbClientsConnectes++;
        return 1; //ajout reussi
    }
    return -1; //tebleau plein
}
int retirerClient(int socketClient)
{
    int pos = estPresent(socketClient);
    if(pos!=-1)
    {
        for(int i=pos; i<nbClientsConnectes-1; i++)
        {
            clientsConnectes[i]=clientsConnectes[i+1];
        }
        nbClientsConnectes--;
        return 1; //retrait reussi
    }
    return -1; //client non present
}


bool OBEP_Parser(char* requete, char* reponse, int socketClient)
{
    // separe les champs de la ligne ';'
    /*stringstream ss(ligne);
    string typeImage, nomFichierImage, nomImage;
    getline(ss, typeImage, ';');
    getline(ss, nomFichierImage, ';');
    getline(ss, nomImage, ';');*/
    istringstream is(requete);
    string TypeRequete;
    getline(is, TypeRequete, '#');
    if(TypeRequete == "LOGIN")
    {
        string user, password;
        getline(is, user, '#');
        getline(is, password, '#');
        
        
        if(estPresent(socketClient)>=0)
        {
            strcpy(reponse, "LOGIN#KO#Client deja loggé");
            return false;
        }
        else
        {
            if(OBEP_Login(user.c_str(), password.c_str()))
            {
                ajouterClient(socketClient);
                strcpy(reponse, "LOGIN#OK#");
                printf("Client %s loggé\n", user.c_str());
                return true;
            }
            else
            {
                strcpy(reponse, "LOGIN#KO#Login ou mot de passe incorrect");
                return false;
            }
        }
        
        
    
    }
    else if(TypeRequete == "LOGOUT")
    {
        retirerClient(socketClient);
        strcpy(reponse, "LOGOUT#OK#");
        return false;
    }
    else if(TypeRequete == "OPER")
    {
        string op;
        string term1, term2;
        getline(is, op, '#');
        getline(is, term1, '#');
        getline(is, term2, '#');

        char opp=op[0];
        int a = atoi((char*)term1.c_str());
        int b = atoi((char*)term2.c_str());
        
        try
        {
            int res = SMOP_Operation( opp, a,b);
            sprintf(reponse, "OPER#OK#%d", res);
        }
        catch(int e)
        {
            strcpy(reponse, "OPER#KO#Division par zero");
        }
        return true;
    }
    
    

}
bool OBEP_Login(const char* user, const char* password)
{
    if(strcmp(user, "admin")==0 && strcmp(password, "admin")==0) return true;
    else if(strcmp(user, "mbaya")==0 && strcmp(password, "student1")==0) return true;
    else if(strcmp(user, "carlisi")==0 && strcmp(password, "student2")==0) return true;
    else return false;
    
}
bool OBEP_Logout(const char* user)
{
    if(strcmp(user, "admin")==0) return true;
    else if(strcmp(user, "mbaya")==0) return true;
    else if(strcmp(user, "carlisi")==0) return true;
    else return false;


}
int SMOP_Operation(char op,int a,int b)
{
if (op == '+') return a+b; if (op == '-') return a-b; if (op == '*') return a*b; if (op == '/')
{
if (b == 0) throw 1; return a/b;
} return 0;
}

void OBEP_Close()
{
    //Fermer les sockets des clients connectes
    for(int i=0; i<nbClientsConnectes; i++)
    {
        close(clientsConnectes[i]);
    }
}
void OBEP_GetAuthors(char * reponse,MYSQL *conn);
void OBEP_GetSubjets(char * reponse,MYSQL *conn);
void OBEP_Add_Author(char * reponse,MYSQL *conn);
void OBEP_Add_Subject(char * reponse,MYSQL *conn);
void OBEP_Add_Book(char * reponse,MYSQL *conn);