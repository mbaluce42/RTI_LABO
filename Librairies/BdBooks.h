#ifndef BDBOOKS_H
#define BDBOOKS_H

#include <string>


using namespace std;



int BdBooks_getAuthors(string& result);
int BdBooks_getIdAuthor(string& result,string prenom, string nom);

int BdBooks_getSubjets(string& result);
int BdBooks_getIdSubject(string& result,string nom);

int bdBooks_getBooks(string& result);
int bdBooks_getIdBook(string& result,string titre);

int BdBooks_getIdEmployee(string& result,string login);

int BdBooks_getEmployees(string& result);
int BdBooks_getEncodedBooks(string& result);


int BdBooks_Add_Author(string& reponse,string nom,string prenom,string date);

int BdBooks_Add_Subject(string& reponse,string nom);

int BdBooks_Add_Book(string& reponse,string author_id,string subject_id,string titre,string isbn,string nbrPage, string stockDisponible, string prix, string anneePublication);

int BdBooks_Add_Employee(string& reponse,string login,string password);

int BdBooks_Add_EncodedBook(string& reponse,string employee_id,string book_id,string date);

#endif