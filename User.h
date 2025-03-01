#pragma once
#include <vector>
#include <string>
#include "Messages.h"

class User {
public:
    std::string password;
    std::string name;
    std::string login;

    std::vector<Message> messages;

    void PrintAllMessages();
};
