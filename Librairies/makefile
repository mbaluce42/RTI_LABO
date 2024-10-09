PROGAMS = serveurTest clientTest

all: $(PROGAMS)

serveurTest: serveurTest.cpp socket.o
			g++ -o serveurTest serveurTest.cpp socket.o

clientTest: clientTest.cpp socket.o
			g++ -o clientTest clientTest.cpp socket.o

socket.o:	socket.cpp socket.h
			g++ socket.cpp -c -DDEBUG

