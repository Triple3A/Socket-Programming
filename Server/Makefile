all: Server

Server : Main.o User.o Server.o
	g++ -lstdc++fs Main.o User.o Server.o -o Server

User.o : User.cpp User.h
	g++ -lstdc++fs -c User.cpp

Main.o : Main.cpp Server.h
	g++ -lstdc++fs -c Main.cpp

Server.o : Server.cpp Server.h User.h Exceptions.h
	g++ -lstdc++fs -c Server.cpp


clean:
	rm *.o Server

