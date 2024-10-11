#ifndef BDBOOKS_H
#define BDBOOKS_H

#include <string>


using namespace std;



int BdBooks_getAuthors(string& result);
int BdBooks_getIdAuthor(string& result,string prenom, string nom);

int BdBooks_getSubjets(string& result);
int BdBooks_getIdSubject(string& result,string nom);

int bdBooks_getBooks(string& result);


int BdBooks_Add_Author(string& reponse,string nom,string prenom,string date);
int BdBooks_Remove_Last_Author(string& result);

int BdBooks_Add_Subject(string& reponse,string nom);
int BdBooks_Remove_Last_Subject(string& reponse);


int BdBooks_Add_Book(string& reponse,string author_id,string subject_id,string titre,string isbn,string nbrPage, string stockDisponible, string prix, string anneePublication);


#endif