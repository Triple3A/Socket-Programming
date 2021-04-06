#include "Server.h"
#include "Exceptions.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "filesystem.h"

using namespace std;
using json = nlohmann::json;

Server::Server(std::string file)
{
	json j;

	ifstream people_file(file, ifstream::binary);
	people_file >> j;

	auto jj = json::parse(j);

	auto uu = jj["users"];
	auto f = jj["files"];

	for (auto a : uu)
	{
		std::string user = a["user"];
		std::string pass = a["password"];
		bool admin = a["admin"];
		int64_t size = a["size"];

		User *u = new User(user, pass, admin, size * 1000);
		users.push_back(u);
	}

	for (auto a : f)
	{
		files.push_back(a);
	}

	currentUser = NULL;
}

void Server::initializeServerSocket()
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
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
	// if (bind(server_fd, (struct sockaddr *)&serverCommandAddr,
	// 		 sizeof(serverCommandAddr)) < 0)
	// {
	// 	perror("bind failed");
	// 	exit(EXIT_FAILURE);
	// }
	bind(server_fd, (struct sockaddr *)&serverCommandAddr, sizeof(serverCommandAddr));
	bind(server_fd, (struct sockaddr *)&serverDataAddr, sizeof(serverDataAddr));

	// if (bind(server_fd, (struct sockaddr *)&serverDataAddr,
	// 		 sizeof(serverDataAddr)) < 0)
	// {
	// 	perror("bind failed");
	// 	exit(EXIT_FAILURE);
	// }

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

std::string Server::firstWord(std::string &line)
{
	int end = line.find(' ');
	std::string ans;
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
	std::string res = "";
	std::string data = "";
	//make socket.

	initializeServerSocket();
	char buffer[1024] = {0};
	int valread = read(new_command_socket, buffer, strlen(buffer));
	std::string commandLine(buffer, strlen(buffer));
	while (commandLine != "")
	{
		//listen to socket.

		std::string command = firstWord(commandLine);
		try
		{
			if (command == "user")
			{
				std::string name = this->firstWord(commandLine);
				if (commandLine != "")
					throw Syntax_Exception();
				currentUser = this->checkUser(name);
				res = "331: User name okay, need password.";
				send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			}
			else if (command == "pass")
			{
				if (currentUser == NULL)
					throw Login_Exception();
				std::string pass = this->firstWord(commandLine);
				if (commandLine != "")
					throw Syntax_Exception();
				this->checkPass(currentUser, pass);
				res = "230:‬‬ ‫‪User‬‬ ‫‪logged‬‬ ‫‪in,‬‬ ‫‪proceed.‬‬ ‫‪Logged‬‬ ‫‪out‬‬ ‫‪if‬‬ ‫‪appropriate.";
				send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			}
			else if (command == "pwd")
			{
				if (commandLine != "")
					throw Syntax_Exception();
				char pwd[1024];
				getcwd(pwd, 1024);
				res = "257: " + std::string(pwd, 1024);
				send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			}
			else if (command == "mkd")
			{
				std::string newd = this->firstWord(commandLine);
				if (commandLine != "")
					throw Syntax_Exception();
				mkdir(newd.c_str(), 0777);
				res = "257: " + newd + " created.";
				send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			}
			else if (command == "dele")
			{
				if (commandLine == "")
					throw Syntax_Exception();
				std::string _command = this->firstWord(commandLine);
				if (commandLine == "")
					throw Syntax_Exception();
				std::string _file = this->firstWord(commandLine);
				if (commandLine != "")
					throw Syntax_Exception();
				checkPermission(currentUser, _file);
				if (_command == "-d")
					rmdir(_file.c_str());
				if (_command == "-f")
					remove(_file.c_str());

				res = "250: <filename/directory path> deleted.";
				send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			}
			else if (command == "ls")
			{
				if (commandLine != "")
					throw Syntax_Exception();
				char pwd[1024];
				getcwd(pwd, 1024);
				for (auto &entry : filesystem::directory_iterator(pwd))
				{
					data = data + std::string(entry.path(), 2000);
					data = data + "\n";
				}
				send(new_data_socket, data.c_str(), strlen(data.c_str()), 0);
				res = "226: List transfer done.";
				send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			}
			else if (command == "cwd")
			{
				if (commandLine == "")
					throw Syntax_Exception();
				std::string path = this->firstWord(commandLine);
				if (commandLine != "")
					throw Syntax_Exception();
				filesystem::current_path(path.c_str());
				res = "250:‬‬ ‫‪Successful‬‬ ‫‪change.";
				send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			}
			else if (command == "rename")
			{
				if (commandLine == "")
					throw Syntax_Exception();
				std::string from = this->firstWord(commandLine);
				if (commandLine == "")
					throw Syntax_Exception();
				std::string to = this->firstWord(commandLine);
				if (commandLine != "")
					throw Syntax_Exception();
				checkPermission(currentUser, from);
				rename(from.c_str(), to.c_str());
				res = "250:‬‬ ‫‪Successful‬‬ ‫‪change.";
				send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			}
			else if (command == "retr")
			{
				if (commandLine == "")
					throw Syntax_Exception();
				std::string name = this->firstWord(commandLine);
				if (commandLine != "")
					throw Syntax_Exception();
				checkPermission(currentUser, name);
				
				ifstream fs(name, ios::binary);
				fs.seekg(0, ios::end);
				int64_t amount = fs.tellg();
				checkVolume(currentUser, amount);
				currentUser->decVolume(amount);
				std::string line;
				std::string data = "";
				while (getline(fs, line))
				{
					data = data + line;
					data = data + "\n";
				}
				send(new_data_socket, data.c_str(), strlen(data.c_str()), 0);
				res = "226:‬‬ ‫‪Successful‬‬ ‫‪Download.";
				send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
			}
			else if (command == "help")
			{
				if (commandLine != "")
					throw Syntax_Exception();
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
			}
			else if (command == "quit")
			{
				res = "221: Successful Quit.\n";
				send(new_command_socket, res.c_str(), strlen(res.c_str()), 0);
				close(new_command_socket);
				close(new_data_socket);
			}
			else
			{
				throw Syntax_Exception();
			}
		}
		catch (User_Pass_Exception e)
		{
			send(new_command_socket, e.what().c_str(), strlen(e.what().c_str()), 0);
			// cout<<e.what()<<endl;
		}
		catch (Syntax_Exception e)
		{
			send(new_command_socket, e.what().c_str(), strlen(e.what().c_str()), 0);
			// cout<<e.what()<<endl;
		}
		catch (Volume_Exception e)
		{
			send(new_command_socket, e.what().c_str(), strlen(e.what().c_str()), 0);
			// cout<<e.what()<<endl;
		}
		catch (Permission_Exception e)
		{
			send(new_command_socket, e.what().c_str(), strlen(e.what().c_str()), 0);
			// cout<<e.what()<<endl;
		}
	}
	return new_command_socket;
}

User *Server::checkUser(std::string name)
{
	int i;
	for (i = 0; i < this->users.size(); i++)
	{
		if (this->users[i]->getName() == name)
			return users[i];
	}
	throw User_Pass_Exception();
}

void Server::checkPass(User *currentUser, std::string pass)
{
	if (currentUser->getPass() != pass)
		throw User_Pass_Exception();
}

void Server::checkPermission(User *currentUser, std::string file)
{
	if (currentUser->isAdmin())
		return;
	int i;
	for (i = 0; i < this->files.size(); i++)
		if (this->files[i] == file)
			throw Permission_Exception();
}

void Server::checkVolume(User* currentUser, int64_t amount)
{
	if (currentUser->getVolume() < amount)
		throw Volume_Exception();
}