PROGAMS = serveur client

all: $(PROGAMS)

serveur:	serveur.cpp ./Librairies/socket.o ./Protocole/Prot_OBEP.o
		g++ -o serveur serveur.cpp ./Librairies/socket.o ./Protocole/Prot_OBEP.o -I/usr/include/mysql

client:	client.cpp ./Librairies/socket.o
		g++ -o client client.cpp ./Librairies/socket.o -I/usr/include/mysql

socket.o:	./Librairies/socket.cpp ./Librairies/socket.h
		g++ ./Librairies/socket.cpp -c -DDEBUG

Prot_OBEP.o:	./Protocole/Prot_OBEP.cpp ./Protocole/Prot_OBEP.h
		g++ ./Protocole/Prot_OBEP.cpp -c -DDEBUG -I/usr/include/mysql