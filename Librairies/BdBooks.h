#ifndef BDBOOKS_H
#define BDBOOKS_H

#include <string>


using namespace std;



int BdBooks_getAuthors(string& result);
int BdBooks_getSubjets(string& result);


void BdBooks_Add_Author(char * reponse);
void BdBooks_Add_Subject(char * reponse);
void BdBooks_Add_Book(char * reponse);


#endif