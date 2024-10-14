PROGAMS = clientTestServeurEncodage serveurEncodage
MYSQL_DIR = /usr/lib64/mysql
MYSQL_INCLUDE = /usr/include/mysql

all: $(PROGAMS)

serveurEncodage: serveurEncodage.cpp ./Librairies/socket.o ./Protocole/Prot_OBEP.o ./Librairies/BdBooks.o
	g++ -o serveurEncodage serveurEncodage.cpp ./Librairies/socket.o ./Protocole/Prot_OBEP.o ./Librairies/BdBooks.o -I$(MYSQL_INCLUDE) -L$(MYSQL_DIR) -lmysqlclient -pthread

clientTestServeurEncodage: clientTestServeurEncodage.cpp ./Librairies/socket.o
	g++ -o clientTestServeurEncodage clientTestServeurEncodage.cpp ./Librairies/socket.o -I$(MYSQL_INCLUDE) -L$(MYSQL_DIR) -lmysqlclient

./Librairies/socket.o: ./Librairies/socket.cpp ./Librairies/socket.h
	g++ ./Librairies/socket.cpp -c -DDEBUG -o ./Librairies/socket.o

./Protocole/Prot_OBEP.o: ./Protocole/Prot_OBEP.cpp ./Protocole/Prot_OBEP.h
	g++ ./Protocole/Prot_OBEP.cpp -c -DDEBUG -I$(MYSQL_INCLUDE) -o ./Protocole/Prot_OBEP.o

./Librairies/BdBooks.o: ./Librairies/BdBooks.cpp ./Librairies/BdBooks.h
	g++ ./Librairies/BdBooks.cpp -c -DDEBUG -I$(MYSQL_INCLUDE) -o ./Librairies/BdBooks.o

clean:
	rm -f $(PROGAMS) ./Librairies/*.o ./Protocole/*.o