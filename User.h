#ifndef USER
#define USER

#include <string.h>

class User:{
private:
	std::string name;
	std::string pass;
	bool admin;
	int volume;

public:
	User();
	std::string getName(){return name;}
	std::string gatPass(){return pass;}
	bool isAdmin(){return admin;}
	int getVolume(){return volume;}
}

#endif