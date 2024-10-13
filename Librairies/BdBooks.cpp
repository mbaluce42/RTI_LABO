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
        result="GET_SUBJECTS#KO#Connexion a la BD impossible\n";
        return -1; //"ERROR BD";
    }
    printf("(GetSubjects)Connexion reussie vers la BD\n");

    string requete = "SELECT * FROM subjects";

    if(mysql_query(connexion, requete.c_str()))
    {
        result="GET_SUBJECTS#KO#Requete SELECT impossible\n";
        return -2;//"ERROR REQUETE";
    }

    printf("(GetSubjects)Requete SELECT reussie\n");

    //recuperation des resultats
    MYSQL_RES *resultat;
    if((resultat=mysql_store_result(connexion))==NULL)
    {
        result="GET_SUBJECTS#KO#Recuperation des resultats impossible\n";
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

int BdBooks_getIdSubject(string& result,string nom)
{
    if(nom.empty())
    {
        result="GETID_SUBJECT#KO#Nom du sujet vide (doit etre GETID_AUTHOR#nomSubject)";
        return -1;
    }


    MYSQL *connexion;
    connexion = mysql_init(NULL);

    mysql_set_character_set(connexion, "utf8");

    //connection a la base de donnees
    if(mysql_real_connect(connexion, "localhost","Student","PassStudent1_","PourStudent",0,NULL,0)==NULL)
    {
        result="GETID_SUBJECT#KO#Connexion a la BD impossible\n";
        return -1; //"ERROR BD";
    }
    printf("(GetIdSubject)Connexion reussie vers la BD\n");

    string requete = "SELECT id FROM subjects WHERE LOWER(name) = LOWER('"+nom+"')";
    if(mysql_query(connexion, requete.c_str()))
    {
        result="GETID_SUBJECT#KO#Requete SELECT impossible\n";
        return -2;//"ERROR REQUETE";
    }

    printf("(GetIdSubject)Requete SELECT reussie\n");

    //recuperation des resultats
    MYSQL_RES *resultat;
    if((resultat=mysql_store_result(connexion))==NULL)
    {
        result="GETID_SUBJECT#KO#Recuperation des resultats impossible\n";
        return -3;//"ERROR RECUP RESULTAT";
    }
    printf("(GetIdSubject)Recuperation des resultats reussie\n");

    MYSQL_ROW ligne;
    ligne=mysql_fetch_row(resultat);
    if(ligne==NULL)
    {
        result="GETID_SUBJECT#KO#Sujet non trouve (doit etre GETID_AUTHOR#nomSubject)";
        return -4;
    }
    ostringstream ossResultat;
    ossResultat<< "GETID_SUBJECT#OK" <<'\n';
    ossResultat << ligne[0] << '\n';
    printf("\n(GetIdSubject)Traitement des resultats reussi\n");
    mysql_free_result(resultat);
    mysql_close(connexion);

    result = ossResultat.str();

    return 0;
}

int BdBooks_getAuthors(string &result)
{
    MYSQL *connexion;
    connexion = mysql_init(NULL);

    mysql_set_character_set(connexion, "utf8");

    // connection a la base de donnees
    if (mysql_real_connect(connexion, "localhost", "Student", "PassStudent1_", "PourStudent", 0, NULL, 0) == NULL)
    {
        result = "GET_AUTHORS#KO#Connexion a la BD impossible";
        return -1; //"ERROR BD";
    }
    printf("(GetAuthors)Connexion reussie vers la BD\n");

    string requete = "SELECT * FROM authors";
    if (mysql_query(connexion, requete.c_str()))
    {
        result = "GET_AUTHORS#KO#Requete SELECT impossible";
        return -2; //"ERROR REQUETE";
    }
    printf("(GetAuthors)Requete SELECT reussie\n");

    // recuperation des resultats
    MYSQL_RES *resultat;
    if ((resultat = mysql_store_result(connexion)) == NULL)
    {
        result = "GET_AUTHORS#KO#Recuperation des resultats impossible";
        return -3; //"ERROR RECUP RESULTAT";
    }
    printf("(GetAuthors)Recuperation des resultats reussie\n");

    MYSQL_ROW ligne;
    int nbrChamps = mysql_num_fields(resultat); // nombre de colonnes
    ostringstream ossResultat;
    ossResultat << "GET_AUTHORS#OK" << '\n';

    while ((ligne = mysql_fetch_row(resultat)) != NULL)
    {
        for (int i = 0; i < nbrChamps; i++)
        {
            ossResultat << ligne[i];
            if (i < nbrChamps - 1)
            {
                ossResultat << ";";
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

int BdBooks_getIdAuthor(string& result,string prenom, string nom)
{

    if(prenom.empty() || nom.empty())
    {
        result="GETID_AUTHOR#KO#Nom ou prenom de l'auteur vide(doit etre GETID_AUTHOR#prenomAuthor nomAuthor)";
        return -1;
    }
    MYSQL *connexion;
    connexion = mysql_init(NULL);

    mysql_set_character_set(connexion, "utf8");

    //connection a la base de donnees
    if(mysql_real_connect(connexion, "localhost","Student","PassStudent1_","PourStudent",0,NULL,0)==NULL)
    {
        result="GETID_AUTHOR#KO#Connexion a la BD impossible";
        return -1; //"ERROR BD";
    }
    printf("(GetIdAuthor)Connexion reussie vers la BD\n");


    string requete = "SELECT id FROM authors WHERE LOWER(last_name) = LOWER('" + nom + "') AND LOWER(first_name) = LOWER('" + prenom + "')";
    if(mysql_query(connexion, requete.c_str()))
    {
        result="GETID_AUTHOR#KO#Requete SELECT impossible";
        return -2;//"ERROR REQUETE";
    }

    printf("(GetIdAuthor)Requete SELECT reussie\n");

    //recuperation des resultats
    MYSQL_RES *resultat;
    if((resultat=mysql_store_result(connexion))==NULL)
    {
        result="GETID_AUTHOR#KO#Recuperation des resultats impossible";
        return -3;//"ERROR RECUP RESULTAT";
    }
    printf("(GetIdAuthor)Recuperation des resultats reussie\n");

    MYSQL_ROW ligne;
    ligne=mysql_fetch_row(resultat);
    if(ligne==NULL)
    {
        result="GETID_AUTHOR#KO#Auteur non trouve (doit etre GETID_AUTHOR#prenomAuthor nomAuthor)";
        return -4;
    }
    ostringstream ossResultat;
    ossResultat<< "GETID_AUTHOR#OK" <<'\n';
    ossResultat << ligne[0] << '\n';
    printf("\n(GetIdAuthor)Traitement des resultats reussi\n");
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

int bdBooks_getIdBook(string& result,string titre)
{
    if(titre.empty())
    {
        result="GETID_BOOK#KO#Titre du livre vide (doit etre GETID_BOOK#titreLivre)";
        return -1;
    }

    MYSQL *connexion;
    connexion = mysql_init(NULL);

    mysql_set_character_set(connexion, "utf8");

    //connection a la base de donnees
    if(mysql_real_connect(connexion, "localhost","Student","PassStudent1_","PourStudent",0,NULL,0)==NULL)
    {
        result="GETID_BOOK#KO#Connexion a la BD impossible";
        return -1; //"ERROR BD";
    }
    printf("(GetIdBook)Connexion reussie vers la BD\n");

    string requete = "SELECT id FROM books WHERE LOWER(title) = LOWER('"+titre+"')";
    if(mysql_query(connexion, requete.c_str()))
    {
        result="GETID_BOOK#KO#Requete SELECT impossible";
        return -2;//"ERROR REQUETE";
    }

    printf("(GetIdBook)Requete SELECT reussie\n");

    //recuperation des resultats
    MYSQL_RES *resultat;
    if((resultat=mysql_store_result(connexion))==NULL)
    {
        result="GETID_BOOK#KO#Recuperation des resultats impossible";
        return -3;//"ERROR RECUP RESULTAT";
    }
    printf("(GetIdBook)Recuperation des resultats reussie\n");

    MYSQL_ROW ligne;
    ligne=mysql_fetch_row(resultat);
    if(ligne==NULL)
    {
        result="GETID_BOOK#KO#Livre non trouve (doit etre GETID_BOOK#titreLivre)";
        return -4;
    }
    ostringstream ossResultat;
    ossResultat<< "GETID_BOOK#OK" <<'\n';
    ossResultat << ligne[0] << '\n';
    printf("\n(GetIdBook)Traitement des resultats reussi\n");
    mysql_free_result(resultat);
    mysql_close(connexion);

    result = ossResultat.str();

    return 0;
}

int BdBooks_getIdEmployee(string& result,string login)
{
    if(login.empty())
    {
        result="GETID_EMPLOYEE#KO#Login de l'employe vide (doit etre GETID_EMPLOYEE#loginEmployee)";
        return -1;
    }

    MYSQL *connexion;
    connexion = mysql_init(NULL);

    mysql_set_character_set(connexion, "utf8");

    //connection a la base de donnees
    if(mysql_real_connect(connexion, "localhost","Student","PassStudent1_","PourStudent",0,NULL,0)==NULL)
    {
        result="GETID_EMPLOYEE#KO#Connexion a la BD impossible";
        return -1; //"ERROR BD";
    }
    printf("(GetIdEmployee)Connexion reussie vers la BD\n");

    string requete = "SELECT id FROM employees WHERE LOWER(login) = LOWER('"+login+"')";
    if(mysql_query(connexion, requete.c_str()))
    {
        result="GETID_EMPLOYEE#KO#Requete SELECT impossible";
        return -2;//"ERROR REQUETE";
    }

    printf("(GetIdEmployee)Requete SELECT reussie\n");

    //recuperation des resultats
    MYSQL_RES *resultat;
    if((resultat=mysql_store_result(connexion))==NULL)
    {
        result="GETID_EMPLOYEE#KO#Recuperation des resultats impossible";
        return -3;//"ERROR RECUP RESULTAT";
    }
    printf("(GetIdEmployee)Recuperation des resultats reussie\n");

    MYSQL_ROW ligne;
    ligne=mysql_fetch_row(resultat);
    if(ligne==NULL)
    {
        result="GETID_EMPLOYEE#KO#Employe non trouve (doit etre GETID_EMPLOYEE#loginEmployee)";
        return -4;
    }
    ostringstream ossResultat;
    ossResultat<< "GETID_EMPLOYEE#OK" <<'\n';
    ossResultat << ligne[0] << '\n';
    printf("\n(GetIdEmployee)Traitement des resultats reussi\n");
    mysql_free_result(resultat);
    mysql_close(connexion);

    result = ossResultat.str();

    return 0;
}

int BdBooks_getEncodedBooks(string& result)
{
    MYSQL *connexion;
    connexion = mysql_init(NULL);

    mysql_set_character_set(connexion, "utf8");


    //connection a la base de donnees
    if(mysql_real_connect(connexion, "localhost","Student","PassStudent1_","PourStudent",0,NULL,0)==NULL)
    {
        result="GET_ENCODED_BOOKS#KO#Connexion a la BD impossible";
        return -1; //"ERROR BD";
    }
    printf("(GetEncodedBooks)Connexion reussie vers la BD\n");

    string requete = "SELECT * FROM encoded_books";

    if(mysql_query(connexion, requete.c_str()))
    {
        result="GET_ENCODED_BOOKS#KO#Requete SELECT impossible";
        return -2;//"ERROR REQUETE";
    }

    printf("(GetEncodedBooks)Requete SELECT reussie\n");

    //recuperation des resultats
    MYSQL_RES *resultat;
    if((resultat=mysql_store_result(connexion))==NULL)
    {
        result="GET_ENCODED_BOOKS#KO#Recuperation des resultats impossible";
        return -3;//"ERROR RECUP RESULTAT";
    }
    printf("(GetEncodedBooks)Recuperation des resultats reussie\n");

    MYSQL_ROW ligne;
    int nbrChamps = mysql_num_fields(resultat); //nombre de colonnes
    ostringstream ossResultat;
    ossResultat<< "GET_ENCODED_BOOKS#OK" <<'\n';

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
    printf("\n(GetEncodedBooks)Traitement des resultats reussi\n");
    mysql_free_result(resultat);
    mysql_close(connexion);

    result = ossResultat.str();
    return 0;
}

int BdBooks_getEmployees(string& result)
{
    MYSQL *connexion;
    connexion = mysql_init(NULL);

    mysql_set_character_set(connexion, "utf8");


    //connection a la base de donnees
    if(mysql_real_connect(connexion, "localhost","Student","PassStudent1_","PourStudent",0,NULL,0)==NULL)
    {
        result="GET_EMPLOYEES#KO#Connexion a la BD impossible";
        return -1; //"ERROR BD";
    }
    printf("(GetEmployees)Connexion reussie vers la BD\n");

    string requete = "SELECT * FROM employees";

    if(mysql_query(connexion, requete.c_str()))
    {
        result="GET_EMPLOYEES#KO#Requete SELECT impossible";
        return -2;//"ERROR REQUETE";
    }

    printf("(GetEmployees)Requete SELECT reussie\n");

    //recuperation des resultats
    MYSQL_RES *resultat;
    if((resultat=mysql_store_result(connexion))==NULL)
    {
        result="GET_EMPLOYEES#KO#Recuperation des resultats impossible";
        return -3;//"ERROR RECUP RESULTAT";
    }
    printf("(GetEmployees)Recuperation des resultats reussie\n");

    MYSQL_ROW ligne;
    int nbrChamps = mysql_num_fields(resultat); //nombre de colonnes
    ostringstream ossResultat;
    ossResultat<< "GET_EMPLOYEES#OK" <<'\n';

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
    printf("\n(GetEmployees)Traitement des resultats reussi\n");
    mysql_free_result(resultat);
    mysql_close(connexion);

    result = ossResultat.str();

    return 0;
}


int BdBooks_Add_Author(string& result, string nom, string prenom, string date)
{
    if(nom.empty() || prenom.empty() || date.empty())
    {
        result="ADD_AUTHOR#KO#Nom, prenom ou date de naissance de l'auteur vide (doit etre ADD_AUTHOR#nomAuthor#prenomAuthor#dateNaiss)";
        return -1;
    }
    //verif que date est bien une date
    if(date.length()!=10)
    {
        result="ADD_AUTHOR#KO#Date de naissance de l'auteur incorrecte (doit etre yyyy-mm-dd)";
        return -1;
    }
    else
    {
        if(date[4]!='-' || date[7]!='-')
        {
            result="ADD_AUTHOR#KO#Date de naissance de l'auteur incorrecte (doit etre yyyy-mm-dd)";
            return -1;
        }
    }
    //met 1er lettre en majuscule
    nom[0]=toupper(nom[0]);
    prenom[0]=toupper(prenom[0]);


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
    /*string requete_verif = "SELECT id FROM authors WHERE last_name = '" + nom + 
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

    mysql_free_result(res);*/

    int res=BdBooks_getIdAuthor(result, prenom, nom);
    if(res==0)
    {
        result="ADD_AUTHOR#KO#L'auteur existe deja";
        return -1;
    }

    

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

    if(nom.empty())
    {
        result="ADD_SUBJECT#KO#Nom du sujet vide (doit etre ADD_SUBJECT#nomSubject)";
        return -1;
    }
    //verifie si nom est bien un nom
    for(int i=0; i<nom.length(); i++)
    {
        if(!isalpha(nom[i]) && nom[i]!=' ')
        {
            result="ADD_SUBJECT#KO#Nom du sujet incorrect (ne doit contenir que des lettres et des espaces)";
            return -1;
        }
    }

    //met 1er lettre en majuscule
    nom[0]=toupper(nom[0]);

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
    /*string requete_verif = "SELECT id FROM subjects WHERE name = '" + nom + "'";
    
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

    mysql_free_result(res);*/

    int res=BdBooks_getIdSubject(result, nom);
    if(res==0)
    {
        result="ADD_SUBJECT#KO#Le sujet existe deja";
        return -1;
    }

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
    //verifs
    if(author_id.empty() || subject_id.empty() || titre.empty() || isbn.empty() || nbrPage.empty() || stockDisponible.empty() || prix.empty() || anneePublication.empty())
    {
        reponse="ADD_BOOK#KO#Un ou plusieurs champs vides (doit etre ADD_BOOK#author_id#subject_id#titre#isbn#nbrPage#stockDisponible#prix#anneePublication)";
        return -1;
    }
    //verif si author_id et subject_id sont bien des entiers
    for(int i=0; i<author_id.length(); i++)
    {
        if(!isdigit(author_id[i]))
        {
            reponse="ADD_BOOK#KO#author_id incorrect (doit etre un entier)";
            return -1;
        }
    }
    for(int i=0; i<subject_id.length(); i++)
    {
        if(!isdigit(subject_id[i]))
        {
            reponse="ADD_BOOK#KO#subject_id incorrect (doit etre un entier)";
            return -1;
        }
    }
    //verif si nbrPage, stockDisponible et anneePublication sont bien des entiers
    for(int i=0; i<nbrPage.length(); i++)
    {
        if(!isdigit(nbrPage[i]))
        {
            reponse="ADD_BOOK#KO#nbrPage incorrect (doit etre un entier)";
            return -1;
        }
    }
    for(int i=0; i<stockDisponible.length(); i++)
    {
        if(!isdigit(stockDisponible[i]))
        {
            reponse="ADD_BOOK#KO#stockDisponible incorrect (doit etre un entier)";
            return -1;
        }
    }

    for(int i=0; i<anneePublication.length(); i++)
    {
        if(!isdigit(anneePublication[i]))
        {
            reponse="ADD_BOOK#KO#anneePublication incorrect (doit etre un entier)";
            return -1;
        }
    }
    //verif si isbn est bien formaté exemple: 978-2123456803
    if(isbn.length()!=14) //
    {
        reponse="ADD_BOOK#KO#isbn incorrect le format doit etre compose de 14 caracteres (exemple 978-2123456803)";
        return -1;
    }
    else
    {
        if(isbn[3]!='-')
        {
            reponse="ADD_BOOK#KO#isbn incorrect le format doit etre compose de 3 chiffres (exemple 978-2123456803)";
            return -1;
        }
        for(int i=0; i<3; i++)
        {
            if(!isdigit(isbn[i]))
            {
                reponse="ADD_BOOK#KO#isbn incorrect le format avant le tiret doit etre compose de 3 chiffres (exemple 978-2123456803)";
                return -1;
            }
        }
        for(int i=4; i<14; i++)
        {
            if(!isdigit(isbn[i]))
            {
                reponse="ADD_BOOK#KO#isbn incorrect le format apres le tiret doit etre compose de 10 chiffres (exemple 978-2123456803)";
                return -1;
            }
        }
    }


    //met 1er lettre en majuscule
    titre[0]=toupper(titre[0]);


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

int BdBooks_Add_Employee(string& reponse,string login,string password)
{
    //verifs
    if(login.empty() || password.empty())
    {
        reponse="ADD_EMPLOYEE#KO#Un ou plusieurs champs vides (doit etre ADD_EMPLOYEE#login#password)";
        return -1;
    }
    //verif si login est bien un login
    for(int i=0; i<login.length(); i++)
    {
        if(!isalnum(login[i]))
        {
            reponse="ADD_EMPLOYEE#KO#login incorrect (doit etre compose de lettres et de chiffres)";
            return -1;
        }
    }
    //verif si password est bien un password
    for(int i=0; i<password.length(); i++)
    {
        if(!isalnum(password[i]))
        {
            reponse="ADD_EMPLOYEE#KO#password incorrect (doit etre compose de lettres et de chiffres)";
            return -1;
        }
    }

    MYSQL *connexion ;
    connexion=mysql_init(NULL);


    if (mysql_real_connect(connexion, "localhost", "Student", "PassStudent1_", "PourStudent", 0, NULL, 0) == NULL)
    {
        reponse = "ADD_EMPLOYEE#KO#Connexion a la BD impossible";
        mysql_close(connexion);
        return -2;
    }

    printf("(AddEmployee)Connexion reussie vers la BD\n");

    // Vérification si l'employé existe déjà
    /*string requete_verif = "SELECT id FROM employees WHERE login = '" + login + "'";

    if (mysql_query(connexion, requete_verif.c_str()))
    {
        reponse = "ADD_EMPLOYEE#KO#Verification de l'existence de l'employe impossible";
        mysql_close(connexion);
        return -3;
    }

    MYSQL_RES *res = mysql_store_result(connexion);
    if (res == NULL)
    {
        reponse = "ADD_EMPLOYEE#KO#Erreur lors de la récupération du résultat";
        mysql_close(connexion);
        return -4;
    }

    MYSQL_ROW ligne = mysql_fetch_row(res);
    if (ligne!=NULL)
    {
        reponse = "ADD_EMPLOYEE#KO#L'employe existe deja";
        mysql_free_result(res);
        mysql_close(connexion);
        return -1;
    }

    mysql_free_result(res);*/
    int res=BdBooks_getIdEmployee(reponse, login);
    if(res==0)
    {
        reponse="ADD_EMPLOYEE#KO#L'employe existe deja";
        return -1;
    }

    // Insert
    string requete = "INSERT INTO employees (login, password) VALUES ('" + login + "','" + password + "')";

    if (mysql_query(connexion, requete.c_str()))
    {
        reponse = "ADD_EMPLOYEE#KO#Requete INSERT impossible";
        mysql_close(connexion);
        return -3;
    }

    printf("(AddEmployee)Requete INSERT reussie\n");
    mysql_close(connexion);
    reponse = "ADD_EMPLOYEE#OK#L'employe a ete ajoute avec succes";
    return 0;
}

int BdBooks_Add_EncodedBook(string& reponse,string employee_id,string book_id,string date)
{
    //verifs
    if(employee_id.empty() || book_id.empty() || date.empty())
    {
        reponse="ADD_ENCODED_BOOK#KO#Un ou plusieurs champs vides (doit etre ADD_ENCODED_BOOK#employee_id#book_id#date)";
        return -1;
    }
    //verif si employee_id et book_id sont bien des entiers
    for(int i=0; i<employee_id.length(); i++)
    {
        if(!isdigit(employee_id[i]))
        {
            reponse="ADD_ENCODED_BOOK#KO#employee_id incorrect (doit etre un entier)";
            return -1;
        }
    }
    for(int i=0; i<book_id.length(); i++)
    {
        if(!isdigit(book_id[i]))
        {
            reponse="ADD_ENCODED_BOOK#KO#book_id incorrect (doit etre un entier)";
            return -1;
        }
    }
    //verif que date est bien une date
    if(date.length()!=10)
    {
        reponse="ADD_ENCODED_BOOK#KO#Date incorrecte (doit etre yyyy-mm-dd)";
        return -1;
    }
    else
    {
        if(date[4]!='-' || date[7]!='-')
        {
            reponse="ADD_ENCODED_BOOK#KO#Date incorrecte (doit etre yyyy-mm-dd)";
            return -1;
        }
    }

    MYSQL *connexion ;
    connexion=mysql_init(NULL);


    if (mysql_real_connect(connexion, "localhost", "Student", "PassStudent1_", "PourStudent", 0, NULL, 0) == NULL)
    {
        reponse = "ADD_ENCODED_BOOK#KO#Connexion a la BD impossible";
        mysql_close(connexion);
        return -2;
    }

    printf("(AddEncodedBook)Connexion reussie vers la BD\n");

    // Vérification si l'employé existe déjà
    string requete_verif = "SELECT id FROM employees WHERE id = " + employee_id;

    if (mysql_query(connexion, requete_verif.c_str()))
    {
        reponse = "ADD_ENCODED_BOOK#KO#Verification de l'existence de l'employe impossible";
        mysql_close(connexion);
        return -3;
    }

    MYSQL_RES *res = mysql_store_result(connexion);
    if (res == NULL)
    {
        reponse = "ADD_ENCODED_BOOK#KO#Erreur lors de la récupération du résultat";
        mysql_close(connexion);
        return -4;
    }

    MYSQL_ROW ligne = mysql_fetch_row(res);
    if (ligne==NULL)
    {
        reponse = "ADD_ENCODED_BOOK#KO#L'employe n'existe pas";
        mysql_free_result(res);
        mysql_close(connexion);
        return -1;
    }

    mysql_free_result(res);

    // Vérification si le livre existe déjà
    requete_verif = "SELECT id FROM books WHERE id = " + book_id;

    if (mysql_query(connexion, requete_verif.c_str()))
    {
        reponse = "ADD_ENCODED_BOOK#KO#Verification de l'existence du livre impossible";
        mysql_close(connexion);
        return -3;
    }

    res = mysql_store_result(connexion);
    if (res == NULL)
    {
        reponse = "ADD_ENCODED_BOOK#KO#Erreur lors de la récupération du résultat";
        mysql_close(connexion);
        return -4;
    }    

    ligne = mysql_fetch_row(res);
    if (ligne==NULL)
    {
        reponse = "ADD_ENCODED_BOOK#KO#Le livre n'existe pas";
        mysql_free_result(res);
        mysql_close(connexion);
        return -1;
    }

    mysql_free_result(res);

    // Insert
    string requete = "INSERT INTO encoded_books (book_id, employee_id, date) VALUES (" + book_id + "," + employee_id + ",'" + date + "')";

    if (mysql_query(connexion, requete.c_str()))
    {
        reponse = "ADD_ENCODED_BOOK#KO#Requete INSERT impossible";
        mysql_close(connexion);
        return -3;
    }

    printf("(AddEncodedBook)Requete INSERT reussie\n");
    mysql_close(connexion);
    reponse = "ADD_ENCODED_BOOK#OK#Le livre encode a ete ajoute avec succes";
    return 0;
}

