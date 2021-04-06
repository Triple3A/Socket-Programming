#ifndef USER
#define USER

#include <string>

class User
{
private:
	std::string name;
	std::string pass;
	bool admin;
	int volume;

public:
	User(std::string _name, std::string _pass, bool _admin, int _size);
	std::string getName() { return name; }
	std::string getPass() { return pass; }
	bool isAdmin() { return admin; }
	int getVolume() { return volume; }
	void decVolume(int amount) { volume-=amount; }
};

#endif
