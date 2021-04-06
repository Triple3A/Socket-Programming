all: Server

Server : Main.o User.o Server.o
	g++ -std=c++17 Main.o User.o Server.o -o Server

User.o : User.cpp User.h
	g++ -std=c++17 -c User.cpp

Main.o : Main.cpp Server.h
	g++ -std=c++17 -c Main.cpp

Server.o : Server.cpp Server.h User.h Exceptions.h filesystem.h nlohmann/json.hpp
	g++ -std=c++17 -c Server.cpp


clean:
	rm *.o Server

