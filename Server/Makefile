all: Server

Server : Main.o User.o Server.o
	g++ Main.o User.o Server.o -o Server

User.o : User.cpp User.h
	g++ -c User.cpp

Main.o : Main.cpp Server.h
	g++ -c Main.cpp

Server.o : Server.cpp Server.h User.h Exceptions.h
	g++ -c Server.cpp


clean:
	rm *.o Server

