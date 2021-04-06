#ifndef SERVER
#define SERVER

#include <string.h>

class Server:{
private:
	String_Handler sh;
	std::vector<User> users;
	std::vector<std::string> files;
	int dataChannelPort;
	int commandChannelPort;

public:
	Server();
	std::string firstWord(std::string &line);
	void run();
	void checkUser(std::string name);
}

#endif