#include "User.h"
#include <string>

using namespace std;

User::User(string _name, string _pass, bool _admin, int64_t _size) {
    name = _name;
    pass = _pass;
    admin = _admin;
    volume = _size;
}

