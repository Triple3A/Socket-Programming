#ifndef SERVER
#define SERVER

#include <string.h>
#include <vector>
#include "User.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <exception>
#include <filesystem>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

// #define HEARTBEAT_MSG "127.0.0.1 8000"
#define SERVER_ADDR "127.0.0.1"
#define SERVER_COMMAND_PORT 8888
#define SERVER_DATA_PORT 8000
#define ADDR "239.255.255.250"

class Server
{
private:
	String_Handler sh;
	std::vector<User> users;
	std::vector<std::string> files;
	int dataChannelPort = 8000;
	int commandChannelPort = 8888;
	int serverSocketFD;
	int new_command_socket;
	int new_data_socket;

public:
	Server(std::string);
	std::string firstWord(std::string &line);
	int run();
	void checkUser(std::string name);
	void initializeServerSocket();
};

#endif
