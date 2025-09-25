#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include "Messages.h"
#include "sha1.h"

class User {
public:
    Mysha::uint* password;
    std::string name;
    std::string login;
    std::vector<Message> messages;

    inline friend std::ostream& operator<<(std::ostream& os, const User& user) {
        os << "Username: " << user.name << ", Login: " << user.login;
        return os;
    }
    void PrintAllMessage();
    
};

