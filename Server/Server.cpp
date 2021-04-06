#include "Server.h"
#include "Exceptions.h"
#include <json/value.h>
#include <fstream>

using namespace std;

Server::Server(string file)
{
	//create server private fields.

	std::ifstream people_file(file, std::ifstream::binary);
	people_file >> uu;

	//The following lines will let you access the indexed objects.
	for(auto a : uu["users"]){
		string user = a["user"];
		string pass = a["password"];
		string admin = a["admin"];
		bool isAdmin;
		if(admin == "true")
			isAdmin = true;
		else
			isAdmin = false;
		string size = a["size"];
		
		User u(user, pass, isAdmin, atoi(size));
		users.push_back(u);
	}				  //Prints the value for "Anna"
	
	for(auto a : uu["files"])
	{
		files.push_back(a);
	}
}

void initializeServerSocket()
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char *hello = "Hello from server";
	// cin >> hello;

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
				   &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in serverCommandAddr;
	memset(&serverCommandAddr, 0, sizeof(serverCommandAddr));
	serverCommandAddr.sin_family = AF_INET;
	serverCommandAddr.sin_port = htons(SERVER_COMMAND_PORT);
	serverCommandAddr.sin_addr.s_addr = INADDR_ANY;

	struct sockaddr_in serverDataAddr;
	memset(&serverDataAddr, 0, sizeof(serverDataAddr));
	serverDataAddr.sin_family = AF_INET;
	serverDataAddr.sin_port = htons(SERVER_DATA_PORT);
	serverDataAddr.sin_addr.s_addr = INADDR_ANY;

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&serverCommandAddr,
			 sizeof(serverCommandAddr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (bind(server_fd, (struct sockaddr *)&serverDataAddr,
			 sizeof(serverDataAddr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	if ((new_command_socket = accept(server_fd, (struct sockaddr *)&serverCommandAddr,
									 (socklen_t *)&addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	if ((new_data_socket = accept(server_fd, (struct sockaddr *)&serverDataAddr,
								  (socklen_t *)&addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
}

string Server::firstWord(string &line)
{
	int end = line.find(' ');
	string ans;
	if (end == -1)
	{
		ans = line;
		line = "";
		return ans;
	}
	ans = line.substr(0, end);
	line = line.substr(end + 1);
	return ans;
}

int Server::run()
{
	string commandLine;
	string res = "";
	string data = "";
	//make socket.

	initializeServerSocket();
	char buffer[1024] = {0};
	int valread = read(new_command_socket, buffer, strlen(buffer));
	commandLine = str(buffer);
	while (commandLine != "down")
	{
		//listen to socket.

		string command = this.sh.firstWord(commandLine);
		switch (command)
		{
		case "user":
			string name = this.sh.firstWord(commandLine);
			if (commandLine != "")
				throw new Syntax‬‬_Exception();
			currentUser = this.checkUser(name)
							  res = "331: User name okay, need password.";
			send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			break;
		case "pass":
			string pass = this.sh.firstWord(commandLine);
			if (commandLine != "")
				throw new Syntax‬‬_Exception();
			this.chekPass(currentUser, pass);
			res = "230:‬‬ ‫‪User‬‬ ‫‪logged‬‬ ‫‪in,‬‬ ‫‪proceed.‬‬ ‫‪Logged‬‬ ‫‪out‬‬ ‫‪if‬‬ ‫‪appropriate.";
			send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			break;
		case "pwd":
			if (commandLine != "")
				throw new Syntax‬‬_Exception();
			char pwd[1024];
			getcwd(pwd, 1024);
			strcpy(res, "257: ");
			strcat(res, pwd);
			send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			break;
		case "mkd":
			string newd = this.sh.firstWord(commandLine);
			if (commandLine != "")
				throw new Syntax‬‬_Exception();
			mkdir(newd);
			strcpy(res, "257: ");
			strcat(res, newd);
			strcat(res, "created.");
			send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			break;
		case "dele":
			if (commandLine == "")
				throw new Syntax‬‬_Exception();
			string _command = this.sh.firstWord(commandLine);
			if (commandLine == "")
				throw new Syntax‬‬_Exception();
			string _file = this.sh.firstWord(commandLine);
			if (commandLine != "")
				throw new Syntax‬‬_Exception();
			if (_command == "-d")
				rmdir(_file);
			if (_command == "-f")
				remove(_file);
			send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			break;
		case "ls":
			if (commandLine != "")
				throw new Syntax‬‬_Exception();
			char pwd[1024];
			getcwd(pwd, 1024);
			for (const auto &entry : filesystem::directory_iterator(pwd))
			{
				data = data + entry.path();
				data = data + "\n"
			}
			send(new_data_socket, data.c_str(), strlen(data.c_str()), 0);
			res = "226: List transfer done.";
			send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			break;
		case "cwd":
			if (commandLine == "")
				throw new Syntax‬‬_Exception();
			string path = this.sh.firstWord(commandLine);
			if (commandLine != "")
				throw new Syntax‬‬_Exception();
			filesystem::current_path(path);
			res = "250:‬‬ ‫‪Successful‬‬ ‫‪change.";
			send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			break;
		case "rename":
			if (commandLine == "")
				throw new Syntax‬‬_Exception();
			string from = this.sh.firstWord(commandLine);
			if (commandLine == "")
				throw new Syntax‬‬_Exception();
			string to = this.sh.firstWord(commandLine);
			if (commandLine != "")
				throw new Syntax‬‬_Exception();
			rename(from, to);
			res = "250:‬‬ ‫‪Successful‬‬ ‫‪change.";
			send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			break;
		case "retr":
			if (commandLine == "")
				throw new Syntax‬‬_Exception();
			string name = this.sh.firstWord(commandLine);
			if (commandLine != "")
				throw new Syntax‬‬_Exception();
			ifstream fs(name);
			string line;
			string data = "";
			while (getline(fs, line))
			{
				data = data + line;
				data = data + "\n";
			}
			send(new_data_socket, data.c_str(), strlen(data.c_str()), 0);
			res = "226:‬‬ ‫‪Successful‬‬ ‫‪Download.";
			send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			break;
		case "help":
			if (commandLine != "")
				throw new Syntax‬‬_Exception();
			res = "214:\nUser [name]: gets username.\n";
			res = res + "pass [password]: gets users password.\n";
			res = res + "pwd: shows present working directory.\n";
			res = res + "mkd [directory_path]: makes a new directory in directory_path.\n";
			res = res + "dele [name]: delets file 'name'.\n";
			res = res + "ls: shows list of files in present working directory.\n";
			res = res + "cwd [path]: changes present working directory to 'path'.\n";
			res = res + "rename [from] [to]: renames file name from 'from' to 'to'.\n";
			res = res + "rets [name]: downloads file 'name'.\n";
			res = res + "help: explains server commands.\n";
			res = res + "quit: takes user out from the system.\n";
			send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			break;
		case "quit":
			res = "221: Successful Quit.\n";
			send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			close(new_command_socket);
			close(new_data_socket);
			break;
		}
	}
}
return new_command_socket;
}

User Server::checkUser(string name) throws Exception
{
	int i;
	for (i = 0; i < this.users.length(); i++)
	{
		if (usres[i].getName() == name)
			return users[i];
	}
	throw new User_Pass_Exception();
}

void Server::checkPass(User currentUser, string pass) throws Exception
{
	if (currentUser.getPass != pass)
		throw new User_Pass_Exception();
}