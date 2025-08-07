#pragma once
#include <vector>
#include "User.h"
#include "Hash_Func.h"
#include "sha1.h"

using namespace Mysha;

class Chat {
public:
    std::vector<User> users;
    User currentLogin;

    void start();
    void PrintUsers();

private:
    void ShowHello();
    void Login();
    void AddNewUser();
    void ShowUserMenu();
    bool CheckPasswordByIndex(std::string index, const std::string& password);
    void PrivateChat();
    void PublicChat();
    UserHashTable table;
};
