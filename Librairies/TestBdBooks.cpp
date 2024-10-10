#include "BdBooks.h"

#include <stdio.h>
#include <stdlib.h>
#include <sstream> // Include the sstream header
#include <iostream>

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
    printf("--------------------------------\n\n\n");

    {
        

        string reponse;
        
        printf("Test de la fonction BdBooks_Add_Author: \n");
        printf("Ajout de l'auteur Mbaya Luce\n");
        int res=BdBooks_Add_Author(reponse,"Mbaya","Luce","2002-09-23");
        if(res==0)
        {
            printf("Resultat de l'ajout:\n %s\n", reponse.c_str());

            //verification de l'ajout
            string result;
            BdBooks_getAuthors(result);
            printf("Resultat verif:\n %s\n", result.c_str());
        }
        else
        {
            printf("Erreur: %d\n", res);
        }

    }
    printf("--------------------------------\n\n\n");

    {
        string reponse;
        
        printf("Test de la fonction BdBooks_Add_Subject:  \n");
        printf("Ajout du sujet Mathematiques\n");
        int res=BdBooks_Add_Subject(reponse,"Mathematiques");
        if(res==0)
        {
            printf("Resultat de l'ajout:\n %s\n", reponse.c_str());

            //verification de l'ajout
            string result;
            BdBooks_getSubjets(result);
            printf("Resultat verif:\n %s\n", result.c_str());
        }
        else
        {
            printf("Erreur: %d\n", res);
        }

    }

    printf("--------------------------------\n\n\n");

    {
        string reponse;
        
        printf("Test de la fonction BdBooks_Add_Book:  \n");
        printf("Ajout du livre 'Les maths pour les nuls'\n");
        int res=BdBooks_Add_Book(reponse,"1","4","Les maths pour les nuls","978-2123456803","300","10","20","2020");
        if(res==0)
        {
            printf("Resultat de l'ajout:\n %s\n", reponse.c_str());

            //verification de l'ajout
            string result;
            bdBooks_getBooks(result);
            
            printf("Resultat verif:\n %s\n", result.c_str());
        }
        else
        {
            printf("Erreur: %d\n %s", res, reponse.c_str());
        }

    }


}

