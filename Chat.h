#pragma once
#include <vector>
#include "User.h"
#include "sha1.h"
#include "Messages.h"
#include <limits>
#include <iostream>
#include <ostream>

using namespace Mysha;

class Chat {
public:
   // void loadDataFromFile();
    //���� � �������
    std::string fileUser = "User.txt";
    std::vector<User> users = loadUsersFromFile(fileUser);
 
    //���� � �����������
    std::string filePublicMessage = "publicMessage.txt";
    //������ �� ����� ���������
   
    std::vector<Message> publicMessage = loadPublicMessageFromFile(filePublicMessage);
    
    User currentLogin;
    
    //������ �� ����� �������������
    std::vector<User> loadUsersFromFile(const std::string& filename);
    //���������� � ���� �������������
    void saveUsersToFile(const std::vector<User>& users, const std::string& filename);

    //������ �� ����� ���������
    std::vector<Message> loadPrivateMessageFromFile(const std::string& filename);
    //���������� � ���� ���������
    void savePrivateMessageToFile(const std::vector<Message>& mess, const std::string& filename);
    void savePublicMessageToFile(Message mess, const std::string& filename);
    std::vector<Message> loadPublicMessageFromFile(const std::string& filename);

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
    void PrintPublicMessage();
   
};

