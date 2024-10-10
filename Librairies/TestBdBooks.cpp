#include "BdBooks.h"

#include <stdio.h>
#include <stdlib.h>
#include <sstream> // Include the sstream header

using namespace std;


int main()
{
    {
        string result;
        
        printf("Test de la fonction BdBooks_getSubjets: \n");
        int res=BdBooks_getSubjets(result);
        if(res==0)
        {
            printf("Resultat:\n %s\n", result.c_str());
        }
        else
        {
            printf("Erreur: %d\n", res);
        }

        //pour recuperer les champs
        istringstream iss(result);
        string ligne;
        getline(iss, ligne, '\n'); //ignore le header de la requête
        while(getline(iss, ligne, '\n'))
        {
            istringstream iss2(ligne);
            string id;
            string nomSubject;
            getline(iss2, id, ';');
            getline(iss2, nomSubject, '\n');

            printf("ID: %d, Nom: %s\n", atoi(id.c_str()), nomSubject.c_str());
        }

    }


    printf("--------------------------------\n\n\n");

    {
        string result;
        printf("Test de la fonction BdBooks_getAuthors: \n");
        int res=BdBooks_getAuthors(result);
        if(res==0)
        {
            printf("Resultat:\n %s\n", result.c_str());
        }
        else
        {
            printf("Erreur: %d\n", res);
        }

        //pour recuperer les champs
        istringstream iss(result);
        string ligne;
        getline(iss, ligne, '\n'); //ignore le header de la requête
        while(getline(iss, ligne, '\n'))
        {
            istringstream iss2(ligne);
            string id;
            string nom, prenom, dateNaissance;
            getline(iss2, id, ';');
            getline(iss2, nom, ';');
            getline(iss2, prenom, ';');
            getline(iss2, dateNaissance, '\n');

            printf("ID: %d, Nom: %s, Prenom: %s, Date de naissance: %s\n", atoi(id.c_str()), nom.c_str(), prenom.c_str(), dateNaissance.c_str());
        }
    }


}

