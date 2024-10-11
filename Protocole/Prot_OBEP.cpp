#include "Prot_OBEP.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include "../Librairies/socket.h"
#include "../Librairies/BdBooks.h"
#include <unistd.h>

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
    string result;
    if(TypeRequete == "LOGIN")
    {
        string user, password;
        getline(is, user, '#');
        getline(is, password, '\n');
        
        
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
                strcpy(reponse, "LOGIN#OK#Le client loggé avec succes");
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
        int res=retirerClient(socketClient);
        if(res==1)
        {
            strcpy(reponse, "LOGOUT#OK#Client deconnecté avec succes");
            printf("Client avec socket %d deconnecté\n", socketClient);
            return true;
        }
        else
        {
            strcpy(reponse, "LOGOUT#KO#Client pas trouve, deconnexion impossible");
            return false;
        }
        
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
    else if(TypeRequete == "GET_AUTHORS")
    {
        result="";
        
        int res =BdBooks_getAuthors(result);
        strcpy(reponse, result.c_str());
        if(res==0)
        {
            return true;
        }
        else
        {
            printf("Erreur BdBooks_getAuthors(): %d\n", res);
            return false;
        }

    }
    else if(TypeRequete=="GETID_AUTHOR")
    {
        string prenom,nom;
        getline(is, prenom, ' ');
        getline(is, nom, '\n');
        result="";
        int res = BdBooks_getIdAuthor(result, prenom, nom);
        strcpy(reponse, result.c_str());
        if(res==0)
        {
            return true;
        }
        else
        {
            printf("Erreur BdBooks_getIdAuthor(): %d\n", res);
            return false;
        }
    }
    
    
    else if(TypeRequete == "ADD_AUTHOR")
    {
        string nom, prenom, date;
        getline(is, nom, '#');
        getline(is, prenom, '#');
        getline(is, date, '\n');
        result="";

        int res = BdBooks_Add_Author(result, nom, prenom, date);
        strcpy(reponse, result.c_str());
        if(res==0)
        {
            return true;
        }
        else
        {
            printf("Erreur BdBooks_Add_Author(): %d\n", res);
            return false;
        }

    }
    else if(TypeRequete == "GET_SUBJECTS")
    {
        result="";
        int res = BdBooks_getSubjets(result);
        strcpy(reponse, result.c_str());
        if(res==0)
        {
            return true;
        }
        else
        {
            printf("Erreur BdBooks_getSubjets(): %d\n", res);
            return false;
        }

    }
    else if(TypeRequete == "GETID_SUBJECT")
    {
        string nom;
        getline(is, nom, '\n');
        result="";
        int res = BdBooks_getIdSubject(result, nom);
        strcpy(reponse, result.c_str());
        if(res==0)
        {
            return true;
        }
        else
        {
            printf("Erreur BdBooks_getIdSubject(): %d\n", res);
            return false;
        }

    }
    else if(TypeRequete == "ADD_SUBJECT")
    {
        string nom;
        getline(is, nom, '\n');
        result="";
        int res = BdBooks_Add_Subject(result, nom);
        strcpy(reponse, result.c_str());
        if(res==0)
        {
            return true;
        }
        else
        {
            printf("Erreur BdBooks_Add_Subject(): %d\n", res);
            return false;
        }

    }
    else if(TypeRequete == "GET_BOOKS")
    {
        result="";
        int res = bdBooks_getBooks(result);
        strcpy(reponse, result.c_str());
        if(res==0)
        {
            return true;
        }
        else
        {
            printf("Erreur bdBooks_getBooks(): %d\n", res);
            return false;
        }

    }
    else if(TypeRequete == "ADD_BOOK")
    {
        string author_id, subject_id, titre, isbn, nbrPage, stockDisponible, prix, anneePublication;
        getline(is, author_id, '#');
        getline(is, subject_id, '#');
        getline(is, titre, '#');
        getline(is, isbn, '#');
        getline(is, nbrPage, '#');
        getline(is, stockDisponible, '#');
        getline(is, prix, '#');
        getline(is, anneePublication, '\n');
        result="";
        int res = BdBooks_Add_Book(result, author_id, subject_id, titre, isbn, nbrPage, stockDisponible, prix, anneePublication);
        strcpy(reponse, result.c_str());
        if(res==0)
        {
            return true;
        }
        else
        {
            printf("Erreur BdBooks_Add_Book(): %d\n", res);
            return false;
        }
    }
    
    else
    {
        strcpy(reponse, "ERREUR#KO#Requete inconnue");
        return true; //normalement doit etre false
    }
    
}
bool OBEP_Login(const char* user, const char* password)
{
    if(strcmp(user, "admin")==0 && strcmp(password, "admin")==0) return true;
    else if(strcmp(user, "mbaya")==0 && strcmp(password, "student1")==0) return true;
    else if(strcmp(user, "carlisi")==0 && strcmp(password, "student1")==0) return true;
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
