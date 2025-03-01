#pragma once
#include <vector>
#include "User.h"

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
    bool CheckPasswordByIndex(int index, const std::string& password);
    void PrivateChat();
    void PublicChat();
};
