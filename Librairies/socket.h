#ifndef SOCKET_H
#define SOCKET_H

#define TAILLE_BUFFER 10000

//PAGE 47 TH

/**********************************/
/*Explication fct ServeurSocket() */
/**********************************/
/* Cette fonction permet de créer un socket serveur
 * Elle prend en paramètre un pointeur sur une chaine de caractère qui représente le port sur lequel le serveur va écouter
 * Elle retourne un entier qui est le descripteur du socket serveur
 */

/*fait un appel à socket() pour créer la socket
o construit l’adresse réseau de la socket par appel à getaddrinfo()
o fait appel à bind() pour lier la socket à l’adresse réseau*/
int ServerSocket(char *port);

/*Expliquation fct ClientSocket() */
/* Cette fonction permet de créer un socket client
 * Elle prend en paramètre un pointeur sur une chaine de caractère qui représente l'adresse IP du serveur
 * Elle prend en paramètre un pointeur sur une chaine de caractère qui représente le port sur lequel le serveur écoute
 * Elle retourne un entier qui est le descripteur du socket client
 */
/*fait appel à socket() pour créer la socket
o construit l’adresse réseau de la socket (avec l’IP et le port du serveur) par appel à la fonction getaddrinfo()
o fait appel à connect() pour se connecter sur le serveur*/
int ClientSocket(char *ipServeur, char *portServeur);

/*Explication fct AcceptSocket() */
/* Cette fonction permet d'accepter une connexion entrante
 * Elle prend en paramètre un entier qui est le descripteur du socket serveur
 * Elle prend en paramètre un pointeur sur une chaine de caractère qui représente l'adresse IP du client
 * Elle retourne un entier qui est le descripteur du socket client
 */
/*fait appel à listen()
o fait appel à accept()
o récupère éventuellement  l’adresse  IP  distante  du  client qui  vient  de  se connecter. 
Cette adresse IP est placée dans ipClient si celui-ci est non NULL. 
S’il est non NULL, ipClient doit pointer vers une zone mémoire capable de recevoir une chaîne de caractères de la taille d’une adresse IP 
(Exemple : « 192.168.228.167 »)*/
int AcceptSocket(int sEcoute, char *ipClient);


/*Explication fct SendSocket() */
/* Cette fonction permet d'envoyer un message sur un socket
 * Elle prend en paramètre un entier qui est le descripteur du socket
 * Elle prend en paramètre un pointeur sur une chaine de caractère qui représente le message à envoyer
 * Elle prend en paramètre un entier qui représente la taille du message
 * Elle retourne un entier qui est le nombre d'octets envoyés
 */
int SendSocket(int sSocket, char *buffer,int taille);


/*Explication fct ReceiveSocket() */
/* Cette fonction permet de recevoir un message sur un socket
 * Elle prend en paramètre un entier qui est le descripteur du socket
 * Elle prend en paramètre un pointeur sur une chaine de caractère qui représente le message reçu
 * Elle retourne un entier qui est le nombre d'octets reçus
 */
int ReceiveSocket(int socket, char *buffer);

#endif