#include "BdBooks.h"
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream> // Add this line to include <sstream>


int BdBooks_getSubjets(string& result)
{
    MYSQL *connexion;
    connexion = mysql_init(NULL);

    mysql_set_character_set(connexion, "utf8");


    //connection a la base de donnees
    if(mysql_real_connect(connexion, "localhost","Student","PassStudent1_","PourStudent",0,NULL,0)==NULL)
    {
        result="GET_SUBJECTS#KO#Connexion a la BD impossible";
        return -1; //"ERROR BD";
    }
    printf("(GetSubjects)Connexion reussie vers la BD\n");

    string requete = "SELECT * FROM subjects";

    if(mysql_query(connexion, requete.c_str()))
    {
        result="GET_SUBJECTS#KO#Requete SELECT impossible";
        return -2;//"ERROR REQUETE";
    }

    printf("(GetSubjects)Requete SELECT reussie\n");

    //recuperation des resultats
    MYSQL_RES *resultat;
    if((resultat=mysql_store_result(connexion))==NULL)
    {
        result="GET_SUBJECTS#KO#Recuperation des resultats impossible";
        return -3;//"ERROR RECUP RESULTAT";
    }
    printf("(GetSubjects)Recuperation des resultats reussie\n");

    MYSQL_ROW ligne;
    int nbrChamps = mysql_num_fields(resultat); //nombre de colonnes
    ostringstream ossResultat;
    ossResultat<< "GET_SUBJECTS#OK" <<'\n';

    while((ligne=mysql_fetch_row(resultat))!=NULL)
    {
        for(int i=0; i<nbrChamps; i++)
        {
            ossResultat << ligne[i];
            if(i<nbrChamps-1)
            {
                ossResultat<< ";";
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

    mysql_set_character_set(connexion, "utf8");


    //connection a la base de donnees
    if(mysql_real_connect(connexion, "localhost","Student","PassStudent1_","PourStudent",0,NULL,0)==NULL)
    {
        result="GET_AUTHORS#KO#Connexion a la BD impossible";
        return -1; //"ERROR BD";
    }
    printf("(GetAuthors)Connexion reussie vers la BD\n");

    string requete = "SELECT * FROM authors";

    if(mysql_query(connexion, requete.c_str()))
    {
        result="GET_AUTHORS#KO#Requete SELECT impossible";
        return -2;//"ERROR REQUETE";
    }

    printf("(GetAuthors)Requete SELECT reussie\n");

    //recuperation des resultats
    MYSQL_RES *resultat;
    if((resultat=mysql_store_result(connexion))==NULL)
    {
        result="GET_AUTHORS#KO#Recuperation des resultats impossible";
        return -3;//"ERROR RECUP RESULTAT";
    }
    printf("(GetAuthors)Recuperation des resultats reussie\n");

    MYSQL_ROW ligne;
    int nbrChamps = mysql_num_fields(resultat); //nombre de colonnes
    ostringstream ossResultat;
    ossResultat<< "GET_AUTHORS#OK" <<'\n';

    while((ligne=mysql_fetch_row(resultat))!=NULL)
    {
        for(int i=0; i<nbrChamps; i++)
        {
            ossResultat << ligne[i];
            if(i<nbrChamps-1)
            {
                ossResultat<< ";";
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

int bdBooks_getBooks(string& result)
{
    MYSQL *connexion;
    connexion = mysql_init(NULL);

    mysql_set_character_set(connexion, "utf8");


    //connection a la base de donnees
    if(mysql_real_connect(connexion, "localhost","Student","PassStudent1_","PourStudent",0,NULL,0)==NULL)
    {
        result="GET_BOOKS#KO#Connexion a la BD impossible";
        return -1; //"ERROR BD";
    }
    printf("(GetBooks)Connexion reussie vers la BD\n");

    string requete = "SELECT * FROM books";

    if(mysql_query(connexion, requete.c_str()))
    {
        result="GET_BOOKS#KO#Requete SELECT impossible";
        return -2;//"ERROR REQUETE";
    }

    printf("(GetBooks)Requete SELECT reussie\n");

    //recuperation des resultats
    MYSQL_RES *resultat;
    if((resultat=mysql_store_result(connexion))==NULL)
    {
        result="GET_BOOKS#KO#Recuperation des resultats impossible";
        return -3;//"ERROR RECUP RESULTAT";
    }
    printf("(GetBooks)Recuperation des resultats reussie\n");

    MYSQL_ROW ligne;
    int nbrChamps = mysql_num_fields(resultat); //nombre de colonnes
    ostringstream ossResultat;
    ossResultat<< "GET_BOOKS#OK" <<'\n';

    while((ligne=mysql_fetch_row(resultat))!=NULL)
    {
        for(int i=0; i<nbrChamps; i++)
        {
            ossResultat << ligne[i];
            if(i<nbrChamps-1)
            {
                ossResultat<< ";";
            }    
        }
        ossResultat << '\n';
    }
    printf("\n(GetBooks)Traitement des resultats reussi\n");
    mysql_free_result(resultat);
    mysql_close(connexion);

    result = ossResultat.str();

    return 0;
}

int BdBooks_Add_Author(string& result, string nom, string prenom, string date)
{
    MYSQL *connexion ;
    connexion=mysql_init(NULL);



    if (mysql_real_connect(connexion, "localhost", "Student", "PassStudent1_", "PourStudent", 0, NULL, 0) == NULL)
    {
        result = "ADD_AUTHOR#KO#Connexion a la BD impossible";
        mysql_close(connexion);
        return -2;
    }

    printf("(AddAuthor)Connexion reussie vers la BD\n");

    // Vérification si l'auteur existe déjà
    string requete_verif = "SELECT id FROM authors WHERE last_name = '" + nom + 
                           "' AND first_name = '" + prenom + "' AND birth_date = '" + date + "'";
    
    if (mysql_query(connexion, requete_verif.c_str()))
    {
        result = "ADD_AUTHOR#KO#Verification de l'existence de l'auteur impossible";
        mysql_close(connexion);
        return -3;
    }

    MYSQL_RES *res = mysql_store_result(connexion);
    if (res == NULL)
    {
        result = "ADD_AUTHOR#KO#Erreur lors de la récupération du résultat";
        mysql_close(connexion);
        return -4;
    }

    MYSQL_ROW ligne = mysql_fetch_row(res);
    if (ligne!=NULL)
    {
        result = "ADD_AUTHOR#KO#L'auteur existe deja";
        mysql_free_result(res);
        mysql_close(connexion);
        return -1;
    }

    mysql_free_result(res);

    // Insertion de l'auteur
    string requete = "INSERT INTO authors (last_name, first_name, birth_date) VALUES ('" +
                     nom + "','" + prenom + "','" + date + "')";
    
    if (mysql_query(connexion, requete.c_str()))
    {
        result = "ADD_AUTHOR#KO#Requete INSERT impossible";
        mysql_close(connexion);
        return -3;
    }

    // Récupération de l'ID de l'auteur nouvellement inséré
    //int new_id = mysql_insert_id(connexion);

    printf("(AddAuthor)Requete INSERT reussie\n");
    mysql_close(connexion);
    result = "ADD_AUTHOR#OK#L'auteur a ete ajoute avec succes";
    return 0; 
}



int BdBooks_Add_Subject(string& result,string nom)
{
    MYSQL *connexion ;
    connexion=mysql_init(NULL);



    if (mysql_real_connect(connexion, "localhost", "Student", "PassStudent1_", "PourStudent", 0, NULL, 0) == NULL)
    {
        result = "ADD_SUBJECT#KO#Connexion a la BD impossible";
        mysql_close(connexion); 
        return -2;
    }

    printf("(AddSubject)Connexion reussie vers la BD\n");

    // Vérification si le sujet existe déjà
    string requete_verif = "SELECT id FROM subjects WHERE name = '" + nom + "'";
    
    if (mysql_query(connexion, requete_verif.c_str()))
    {
        result = "ADD_SUBJECT#KO#Verification de l'existence du sujet impossible";
        mysql_close(connexion);
        return -3;
    }

    MYSQL_RES *res = mysql_store_result(connexion);
    if (res == NULL)
    {
        result = "ADD_SUBJECT#KO#Erreur lors de la récupération du résultat";
        mysql_close(connexion);
        return -4;
    }

    MYSQL_ROW ligne = mysql_fetch_row(res);
    if (ligne!=NULL)
    {
        result = "ADD_SUBJECT#KO#Le sujet existe deja";
        mysql_free_result(res);
        mysql_close(connexion);
        return -1; 
    }

    mysql_free_result(res);

    // Insertion du sujet
    string requete = "INSERT INTO subjects (name) VALUES ('" + nom + "')";
    
    if (mysql_query(connexion, requete.c_str()))
    {
        result = "ADD_SUBJECT#KO#Requete INSERT impossible";
        mysql_close(connexion);
        return -3;
    }

    // Récupération de l'ID de l'auteur nouvellement inséré
    //int new_id = mysql_insert_id(connexion);

    printf("(AddSubject)Requete INSERT reussie\n");
    mysql_close(connexion);
    result = "ADD_SUBJECT#OK#Le sujet a ete ajoute avec succes";
    return 0;  
}

int BdBooks_Add_Book(string& reponse,string author_id,string subject_id,string titre,string isbn,string nbrPage, string stockDisponible, string prix, string anneePublication)
{
    MYSQL *connexion ;
    connexion=mysql_init(NULL);


    if (mysql_real_connect(connexion, "localhost", "Student", "PassStudent1_", "PourStudent", 0, NULL, 0) == NULL)
    {
        reponse = "ADD_BOOK#KO#Connexion a la BD impossible";
        mysql_close(connexion);
        return -2;
    }

    printf("(AddBook)Connexion reussie vers la BD\n");

    // Vérification si l'auteur existe déjà
    string requete_verif = "SELECT id FROM authors WHERE id = " + author_id;
    
    if (mysql_query(connexion, requete_verif.c_str()))
    {
        reponse = "ADD_BOOK#KO#Verification de l'existence de l'auteur impossible";
        mysql_close(connexion);
        return -3;
    }

    MYSQL_RES *res = mysql_store_result(connexion);
    if (res == NULL)
    {
        reponse = "ADD_BOOK#KO#Erreur lors de la récupération du résultat";
        mysql_close(connexion);
        return -4;
    }

    MYSQL_ROW ligne = mysql_fetch_row(res);
    if (ligne==NULL)
    {
        reponse = "ADD_BOOK#KO#L'auteur n'existe pas";
        mysql_free_result(res);
        mysql_close(connexion);
        return -1;
    }

    mysql_free_result(res);

    // Vérification si le sujet existe déjà
    requete_verif = "SELECT id FROM subjects WHERE id = " + subject_id;
    
    if (mysql_query(connexion, requete_verif.c_str()))
    {
        reponse = "ADD_BOOK#KO#Verification de l'existence du sujet impossible";
        mysql_close(connexion);
        return -3;
    }

    res = mysql_store_result(connexion);
    if (res == NULL)
    {
        reponse = "ADD_BOOK#KO#Erreur lors de la récupération du résultat";
        mysql_close(connexion);
        return -4;
    }

    ligne = mysql_fetch_row(res);
    if (ligne==NULL)
    {
        reponse = "ADD_BOOK#KO#Le sujet n'existe pas";
        mysql_free_result(res);
        mysql_close(connexion);
        return -1;
    }

    mysql_free_result(res);

    // Insertion du livre
    string requete = "INSERT INTO books (author_id, subject_id, title, isbn, page_count, stock_quantity, price, publish_year) VALUES (" +
                     author_id + "," + subject_id + ",'" + titre + "','" + isbn + "'," + nbrPage + "," + stockDisponible + "," + prix + "," + anneePublication + ")";
    
    if (mysql_query(connexion, requete.c_str()))
    {
        reponse = "ADD_BOOK#KO#Requete INSERT impossible";
        mysql_close(connexion);
        return -3;
    }

    // Récupération de l'ID du livre nouvellement inséré
    //int new_id = mysql_insert_id(connexion);

    printf("(AddBook)Requete INSERT reussie\n");
    mysql_close(connexion);
    reponse = "ADD_BOOK#OK#Le livre a ete ajoute avec succes";
    return 0;
}