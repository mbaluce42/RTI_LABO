#include "BdBooks.h"
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream> // Add this line to include <sstream>


int BdBooks_getSubjets(string& result)
{
    MYSQL *connexion;
    connexion = mysql_init(NULL);

    //connection a la base de donnees
    if(mysql_real_connect(connexion, "localhost","Student","PassStudent1_","PourStudent",0,NULL,0)==NULL)
    {
        return -1; //"ERROR BD";
    }
    printf("(GetSubjects)Connexion reussie vers la BD\n");

    string requete = "SELECT * FROM subjects";

    if(mysql_query(connexion, requete.c_str()))
    {
        return -2;//"ERROR REQUETE";
    }

    printf("(GetSubjects)Requete SELECT reussie\n");

    //recuperation des resultats
    MYSQL_RES *resultat;
    if((resultat=mysql_store_result(connexion))==NULL)
    {
        return -3;//"ERROR RECUP RESULTAT";
    }
    printf("(GetSubjects)Recuperation des resultats reussie\n");

    MYSQL_ROW ligne;
    int nbrChamps = mysql_num_fields(resultat); //nombre de colonnes
    ostringstream ossResultat;
    ossResultat<< "GET_SUBJECTS#" <<'\n';

    while((ligne=mysql_fetch_row(resultat))!=NULL)
    {
        for(int i=0; i<nbrChamps; i++)
        {
            ossResultat << ligne[i];
            if(i<nbrChamps-1)
            {
                ossResultat<< ';';
            }
            
        }
        ossResultat << '\n';
    }
    printf("(GetSubjects)Traitement des resultats reussi\n");
    mysql_free_result(resultat);
    mysql_close(connexion);

    result = ossResultat.str();

    return 0;
    
}

int BdBooks_getAuthors(string& result)
{
    MYSQL *connexion;
    connexion = mysql_init(NULL);

    //connection a la base de donnees
    if(mysql_real_connect(connexion, "localhost","Student","PassStudent1_","PourStudent",0,NULL,0)==NULL)
    {
        return -1; //"ERROR BD";
    }
    printf("(GetAuthors)Connexion reussie vers la BD\n");

    string requete = "SELECT * FROM authors";

    if(mysql_query(connexion, requete.c_str()))
    {
        return -2;//"ERROR REQUETE";
    }

    printf("(GetAuthors)Requete SELECT reussie\n");

    //recuperation des resultats
    MYSQL_RES *resultat;
    if((resultat=mysql_store_result(connexion))==NULL)
    {
        return -3;//"ERROR RECUP RESULTAT";
    }
    printf("(GetAuthors)Recuperation des resultats reussie\n");

    MYSQL_ROW ligne;
    int nbrChamps = mysql_num_fields(resultat); //nombre de colonnes
    ostringstream ossResultat;
    ossResultat<< "GET_AUTHORS#" <<'\n';

    while((ligne=mysql_fetch_row(resultat))!=NULL)
    {
        for(int i=0; i<nbrChamps; i++)
        {
            ossResultat << ligne[i];
            if(i<nbrChamps-1)
            {
                ossResultat<< ';';
            }    
        }
        ossResultat << '\n';
    }
    printf("\n(GetAuthors)Traitement des resultats reussi\n");
    mysql_free_result(resultat);
    mysql_close(connexion);

    result = ossResultat.str();

    return 0;
}