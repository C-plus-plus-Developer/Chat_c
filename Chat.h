#pragma once
#include "User.h"
#include "Messages.h"
#include <limits>
#include <ostream>

using namespace Mysha;

class Chat {
public:
    User currentLogin;

    std::string end = " /end.\n";
    std::string two = "#: ";
   
    //Файл с юзерами
    std::string fileUser = "User.txt";
    std::vector<User> users = loadUsersFromFile(fileUser);
 
    //файл с сообщениями
    std::string filePublicMessage = "publicMessage.txt";
    //Чтение из файла сообщений
    std::vector<Message> publicMessage = loadPublicMessageFromFile(filePublicMessage);
  
    //Чтение из файла пользователей
    std::vector<User> loadUsersFromFile(const std::string& filename);
    //Сохранение в файл пользователей
    void saveUsersToFile(const std::vector<User>& users, const std::string& filename);

   
    //Сохранение в файл публичных сообщений
    void savePublicMessageToFile(const vector<Message>& mess, const std::string& filename);
    std::vector<Message> loadPublicMessageFromFile(const std::string& filename);

    void savePrivateMessageToFile(const std::vector<Message>& mess, const std::string& filename);
    std::vector<Message> loadPrivateMessageFromFile(const std::string& filename);
    void PrintPrivateMessage(int userIndex);


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

