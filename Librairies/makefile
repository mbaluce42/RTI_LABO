PROGAMS = serveurTest clientTest TestBdBooks

all: $(PROGAMS)

serveurTest: serveurTest.cpp socket.o
			g++ -o serveurTest serveurTest.cpp socket.o

TestBdBooks: TestBdBooks.cpp BdBooks.o
			g++ -o TestBdBooks TestBdBooks.cpp BdBooks.o -DDEBUG -L/usr/lib64/mysql -lmysqlclient
		
BdBooks.o: BdBooks.cpp BdBooks.h
			g++ BdBooks.cpp -c -DDEBUG -I/usr/include/mysql

clientTest: clientTest.cpp socket.o
			g++ -o clientTest clientTest.cpp socket.o

socket.o:	socket.cpp socket.h
			g++ socket.cpp -c -DDEBUG



