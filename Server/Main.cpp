#include <iostream>
#include "Server.h"

using namespace std;

const string CONFIG_FILE = "../config.json";

int main() {
    Server server(CONFIG_FILE);
    int newSocket;
    try
    {
        newSocket = server.run();
    }
    catch(const std::exception& e)
    {
        send(newSocket, e.what(), strlen(e.what()), 0);
        // std::cerr << e.what() << '\n';
    }
    return 0;
}