#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include "Messages.h"
#include "sha1.h"
#include <sstream>

struct User {
   
    Mysha::uint* password;
    std::string name;
    std::string login;
    
    std::string end = " /end.\n";
    std::string two = "#: ";

    std::string filePrivateMessage;
    
    std::vector<Message> privateMessages;
   
    //Чтение из файла приватных сообщений
   
    //void PrintPrivateMessage();
   
};

