#ifndef EXCEPTIONS
#define EXCEPTIONS

class User_Pass_Exception: public Exception{
public:
	std::string what(){return "430: Invalid‬‬ ‫‪username‬‬ ‫‪or‬‬ ‫‪password";}
}

class Syntax‬‬_Exception: public Exception{
public:
	std::string what(){return "501:‬‬ ‫‪Syntax‬‬ ‫‪error‬‬ ‫‪in‬‬ ‫‪parameters‬‬ ‫‪or‬‬ ‫‪arguments.";}
}

class Volume_Exception: public Exception{
public:
	std::string what(){return "425:‬‬ ‫‪Can't‬‬ ‫‪open‬‬ ‫‪data‬‬ ‫‪connection.";}
}


#endif