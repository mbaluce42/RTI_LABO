#ifndef BDBOOKS_H
#define BDBOOKS_H

#include <string>


using namespace std;


int BdBooks_getAuthors(string& result);
int BdBooks_getIdAuthorByPrenomNom(string& result,string prenom, string nom);
int BdBooks_Add_Author(string& reponse,string nom,string prenom,string date);

int BdBooks_getSubjets(string& result);
int BdBooks_getIdSubjectByNom(string& result,string nom);
int BdBooks_Add_Subject(string& reponse,string nom);

int bdBooks_getBooks(string& result);
int bdBooks_getBookById(string& result,string book_id);
int bdBooks_getIdBookByLivre(string& result,string titre);

int BdBooks_Add_Book(string& reponse,string author_id,string subject_id,string titre,string isbn,string nbrPage, string stockDisponible, string prix, string anneePublication);

int BdBooks_getEmployees(string& result);
int BdBooks_getIdEmployeeByLogin(string& result,string login);
int BdBooks_Add_Employee(string& reponse,string login,string password);


int BdBooks_getEncodedBooks(string& result);
int BdBooks_getEncodedBooksByEmployee(string& result,string employee_id); //get all books encoded by an employee
int BdBooks_Add_EncodedBook(string& reponse,string employee_id,string book_id,string date);


#endif