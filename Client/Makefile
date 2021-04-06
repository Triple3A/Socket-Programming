all : Client

Client : Main.o Client.o
		g++ Main.o Client.o -o Client

Main.o : Main.cpp Client.h
	g++ -c Main.cpp

Client.o : Client.cpp Client.h
	g++ -c Client.cpp

clean:
	rm *.o Client

