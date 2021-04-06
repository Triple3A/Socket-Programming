#ifndef USER
#define USER

#include <string>

class User
{
private:
	std::string name;
	std::string pass;
	bool admin;
	int64_t volume;

public:
	User(std::string _name, std::string _pass, bool _admin, int64_t _size);
	std::string getName() { return name; }
	std::string getPass() { return pass; }
	bool isAdmin() { return admin; }
	int64_t getVolume() { return volume; }
	void decVolume(int64_t amount) { volume -= amount; }
};

#endif
