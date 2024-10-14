#ifndef PROT_OBEP_H
#define PROT_OBEP_H

#include <mysql.h>

#include <string>

using namespace std;

#define NB_MAX_CLIENTS 100

bool OBEP_Parser(char* requete, char* reponse, int socketClient);
bool OBEP_Parser(string requete, string& reponse, int socketClient);
bool OBEP_Login(const char* user, const char* password);
bool OBEP_Logout(const char* user);
void OBEP_Close();
int SMOP_Operation(char op,int a,int b);

int OBEP_OpClient(char* requete, string& resultat, int sClient);
int Echange(char *requete, char *reponse, int serviceSocket);


int estPresent(int socketClient);
int ajouterClient(int socketClient);
int retirerClient(int socketClient);


#endif