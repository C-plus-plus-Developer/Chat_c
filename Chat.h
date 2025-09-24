#pragma once
#include <vector>
#include "User.h"
#include "Hash_Func.h"
#include "sha1.h"
#include "Messages.h"

using namespace Mysha;

class Chat {
public:
    void loadDataFromFile();
    //���� � �������
    std::string fileUser = "User.txt";
    std::vector<User> users = loadUsersFromFile(fileUser);;
 
    //���� � �����������
    std::string filePublicMessage = "publicMessage.txt";
    //������ �� ����� ���������
    std::vector<Message> loadPublicMessageFromFile(const std::string& filename);
    std::vector<Message> publicMessage;
    
    User currentLogin;
    
    //������ �� ����� �������������
    std::vector<User> loadUsersFromFile(const std::string& filename);
    //���������� � ���� �������������
    void saveUsersToFile(User user, const std::string& filename);

    //������ �� ����� ���������
    std::vector<Message> loadPrivateMessageFromFile(const std::string& filename);
    //���������� � ���� ���������
    void savePrivateMessageToFile(const std::vector<Message>& mess, const std::string& filename);
    void savePublicMessageToFile(const std::vector<Message>& mess, const std::string& filename);
        

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
    UserHashTable table;
};


//���������� � ���� ���������