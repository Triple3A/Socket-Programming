#include <iostream>
#include <fstream>
#include <string.h>
#include <exception>
#include <filesystem>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

Server::Server(){
	//create server private fields.
}

string Server::firstWord(string &line){
	int end=line.find(' ');
	string ans;
	if(end==-1){
		ans=line;
		line="";
		return ans;
	}
	ans=line.substr(0,end);
	line=line.substr(end+1);
	return ans;
}

void Server::run(){
	string commandLine;
	string res="";
	string data="";
	//make socket.
	while(commandLine!="down"){
		//listen to socket.
		string command=this.sh.firstWord(commandLine);
		switch(command){
			case "user":
				string name=this.sh.firstWord(commandLine);
				if(commandLine!="") throw new Syntax‬‬_Exception();
				currentUser=this.checkUser(name)
				res="331: User name okay, need password."
				break;
			case "pass":
				string pass=this.sh.firstWord(commandLine);
				if(commandLine!="") throw new Syntax‬‬_Exception();
				this.chekPass(currentUser,pass);
				res="230:‬‬ ‫‪User‬‬ ‫‪logged‬‬ ‫‪in,‬‬ ‫‪proceed.‬‬ ‫‪Logged‬‬ ‫‪out‬‬ ‫‪if‬‬ ‫‪appropriate.";
				break;
			case "pwd":
				if(commandLine!="") throw new Syntax‬‬_Exception();
				char pwd[1024];
				getcwd(pwd,1024);
				strcpy(res,"257: ");
				strcat(res,pwd);
				break;
			case "mkd":
				string newd=this.sh.firstWord(commandLine);
				if(commandLine!="") throw new Syntax‬‬_Exception();
				mkdir(newd);
				strcpy(res,"257: ");
				strcat(res,newd);
				strcat(res,"created.");
				break;
			case "dele":
				if(commandLine=="") throw new Syntax‬‬_Exception();
				string _command=this.sh.firstWord(commandLine);
				if(commandLine=="") throw new Syntax‬‬_Exception();
				string _file=this.sh.firstWord(commandLine);
				if(commandLine!="") throw new Syntax‬‬_Exception();
				if(_command=="-d") rmdir(_file);
				if(_command=="-f") remove(_file);
				break;
			case "ls":
				if(commandLine!="") throw new Syntax‬‬_Exception();
				char pwd[1024];
				getcwd(pwd,1024);
				for (const auto & entry : filesystem::directory_iterator(pwd)){
        			data=data+entry.path();
        			data=data+"\n"
    			}
    			res="226: List transfer done.";
				break;
			case "cwd":
				if(commandLine=="") throw new Syntax‬‬_Exception();
				string path=this.sh.firstWord(commandLine);
				if(commandLine!="") throw new Syntax‬‬_Exception();
				filesystem::current_path(path);
				res="250:‬‬ ‫‪Successful‬‬ ‫‪change.";
				break;
			case "rename":
				if(commandLine=="") throw new Syntax‬‬_Exception();
				string from=this.sh.firstWord(commandLine);
				if(commandLine=="") throw new Syntax‬‬_Exception();
				string to=this.sh.firstWord(commandLine);
				if(commandLine!="") throw new Syntax‬‬_Exception();
				rename(from,to);
				res="250:‬‬ ‫‪Successful‬‬ ‫‪change.";
				break;
			case "retr":
				if(commandLine=="") throw new Syntax‬‬_Exception();
				string name=this.sh.firstWord(commandLine);
				if(commandLine!="") throw new Syntax‬‬_Exception();
				ifstream fs(name);
				string line;
				string data="";
				while(getline(fs,line)){
					data=data+line;
					data=data+"\n";
				}
				res="226:‬‬ ‫‪Successful‬‬ ‫‪Download.";
				break;
			case "help":
				if(commandLine!="") throw new Syntax‬‬_Exception();
				res="214:\nUser [name]: gets username.\n";
				res=res+"pass [password]: gets users password.\n";
				res=res+"pwd: shows present working directory.\n";
				res=res+"mkd [directory_path]: makes a new directory in directory_path.\n";
				res=res+"dele [name]: delets file 'name'.\n";
				res=res+"ls: shows list of files in present working directory.\n";
				res=res+"cwd [path]: changes present working directory to 'path'.\n";
				res=res+"rename [from] [to]: renames file name from 'from' to 'to'.\n";
				res=res+"rets [name]: downloads file 'name'.\n";
				res=res+"help: explains server commands.\n";
				res=res+"quit: takes user out from the system.\n";
				break;
			case "quit":
				break;
		}
	}
}

User Server::checkUser(string name) throws Exception{
	int i;
	for(i=0;i<this.users.length();i++){
		if(usres[i].getName()==name)
			return users[i];
	}
	throw new User_Pass_Exception();
}

void Server::checkPass(User currentUser,string pass) throws Exception{
	if(currentUser.getPass!=pass)
		throw new User_Pass_Exception();
}