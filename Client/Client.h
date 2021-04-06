#ifndef CLIENT
#define CLIENT

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

#define SERVER_COMMAND_PORT 8888
#define SERVER_DATA_PORT 8000

class Client {
private:
    int command_sock;
    int data_sock;

public:
    Client();
    void initialSocket();
    void run();
};


#endif // !Client
