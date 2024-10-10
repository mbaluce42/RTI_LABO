#ifndef PROT_OBEP_H
#define PROT_OBEP_H

#include <mysql.h>


#define NB_MAX_CLIENTS 100

bool OBEP_Parser(char* requete, char* reponse, int socketClient);
bool OBEP_Login(const char* user, const char* password);
bool OBEP_Logout(const char* user);
void OBEP_Close();
int SMOP_Operation(char op,int a,int b);

/*void OBEP_GetAuthors(string& reponse,MYSQL *conn);
void OBEP_GetSubjets(string& reponse,MYSQL *conn);
void OBEP_Add_Author(string& reponse,MYSQL *conn);
void OBEP_Add_Subject(string& reponse,MYSQL *conn);
void OBEP_Add_Book(string& reponse,MYSQL *conn);*/

int estPresent(int socketClient);
int ajouterClient(int socketClient);
int retirerClient(int socketClient);


#endif