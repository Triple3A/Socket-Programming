#include "Client.h"

using namespace std;


Client::Client() {
    command_sock = 0;
    data_sock = 0;
}

void Client::initialSocket() {
    int valread;
	struct sockaddr_in serv_command_addr;
    struct sockaddr_in serv_data_addr;
	char *hello = "Hello from client";
    // cin >> hello;
	char buffer[1024] = {0};
	if ((command_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		exit(EXIT_FAILURE);
	}

    if ((data_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		exit(EXIT_FAILURE);
	}

	serv_command_addr.sin_family = AF_INET;
	serv_command_addr.sin_port = htons(SERVER_COMMAND_PORT);

    serv_data_addr.sin_family = AF_INET;
	serv_data_addr.sin_port = htons(SERVER_DATA_PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_command_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		exit(EXIT_FAILURE);
	}
    if(inet_pton(AF_INET, "127.0.0.1", &serv_data_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		exit(EXIT_FAILURE);
	}

	if (connect(command_sock, (struct sockaddr *)&serv_command_addr, sizeof(serv_command_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		exit(EXIT_FAILURE);
	}
    if (connect(data_sock, (struct sockaddr *)&serv_data_addr, sizeof(serv_data_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		exit(EXIT_FAILURE);
	}

}

void Client::run() {
    initialSocket();

    string command;
    while(getline(cin, command))
    {
        send(command_sock, command.c_str(), strlen(command.c_str()), 0);
        char command_buf[1024];
        char data_buf[3000];
        int valread1 = read( command_sock , command_buf, 1024);
        int valread2 = read( data_sock, data_buf, 3000);
        string data = (string)data_buf;//str(data_buf);
        string res = (string)command_buf;//str(command_buf);

        if(data != "")
            cout << data << '\n';
        
        cout << res << '\n';
    }
        
}