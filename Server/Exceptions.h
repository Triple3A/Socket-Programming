#ifndef EXCEPTIONS
#define EXCEPTIONS

#include <exception>
#include <iostream>

class User_Pass_Exception: public std::exception{
public:
	std::string what(){return "430: Invalid‬‬ ‫‪username‬‬ ‫‪or‬‬ ‫‪password";}
};

class Login_Exception: public std::exception{
public:
	std::string what(){return "503: Bad sequence of commands.";}
};


class Syntax_Exception: public std::exception{
public:
	std::string what(){return "501:‬‬ ‫‪Syntax‬‬ error‬‬ ‫‪in‬‬ ‫‪parameters‬‬ ‫‪or‬‬ ‫‪arguments.";}
};

class Volume_Exception: public std::exception{
public:
	std::string what(){return "425:‬‬ ‫‪Can't‬‬ ‫‪open‬‬ ‫‪data‬‬ ‫‪connection.";}
};

class Permission_Exception: public std::exception{
public:
	std::string what(){return "550: File‬‬ ‫‪unavailable.";}
};


#endif