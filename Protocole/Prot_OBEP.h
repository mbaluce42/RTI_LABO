#ifndef PROT_OBEP_H
#define PROT_OBEP_H

#include <mysql.h>


#define NB_MAX_CLIENTS 100

bool OBEP_Parser(char* requete, char* reponse, int socketClient);
bool OBEP_Login(const char* user, const char* password);
bool OBEP_Logout(const char* user);
void OBEP_Close();
int SMOP_Operation(char op,int a,int b);

void OBEP_GetAuthors(char * reponse,MYSQL *conn);
void OBEP_GetSubjets(char * reponse,MYSQL *conn);
void OBEP_Add_Author(char * reponse,MYSQL *conn);
void OBEP_Add_Subject(char * reponse,MYSQL *conn);
void OBEP_Add_Book(char * reponse,MYSQL *conn);

int estPresent(int socketClient);
int ajouterClient(int socketClient);
int retirerClient(int socketClient);


#endif