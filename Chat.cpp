#include "Chat.h"

using namespace Mysha;
void Chat::ShowHello() {
    std::cout << "Hello, I am chat" << std::endl;
}

void Chat::Login() {
    std::string userlogin;
    int userIndex;
    std::cout << "Choice concret user" << std::endl;
    PrintUsers();

    std::cout << "Write user index: ";
    std::cin >> userIndex;

    std::cout << "Write user login: ";
    std::cin >> userlogin;

    std::cout << "Write user password: ";
    std::string pass;
    std::cin >> pass;
    
    if (CheckPasswordByIndex(userlogin, pass)) {
        std::cout << "You have entered to user" << std::endl;
        currentLogin = users[userIndex];
        ShowUserMenu();
    }
    
}

void Chat::AddNewUser() {
    std::cout << "Write login, password, name for new user:" << std::endl;
    std::cout << "login: ";
    std::string newLogin;
    std::cin >> newLogin;

    std::cout << "password: ";
    std::string newPass;
    std::cin >> newPass;
    //хеширование пароля
    uint* PassHash = sha1(newPass.c_str(), static_cast <uint>(newPass.size()));

    std::cout << "name: ";
    std::string newName;
    std::cin >> newName;

    User newUser;
   
    newUser.name = newName;
    //сохранение хэша и логина в таблицу
    table.add(newLogin, PassHash);
 
   
    users.push_back(newUser);
    
    std::cout << "User added successfully!" << std::endl;
}

void Chat::start() {
    ShowHello();

    while (true) {
        std::cout << "Write command:" << std::endl;
        std::cout << "1 - Login to chat; " << std::endl;
        std::cout << "2 - Registration new user" << std::endl;
        std::cout << "3 - End" << std::endl;

        int command;
        std::cin >> command;
        switch (command) {
        case 1:
            Login();
            break;
        case 2:
            AddNewUser();
            break;
        case 3:
            std::cout << " Bye, bye" << std::endl;
            return;
        }
    }
}

void Chat::PrintUsers() {
    std::cout << "All users: " << std::endl;
    for (int i = 0; i < users.size(); ++i) {
        std::cout << i << " - " << users[i].name << std::endl;
    }
}

void Chat::ShowUserMenu() {
    while (true) {
        std::cout << "Hi, " << currentLogin.name << std::endl;
        std::cout << "Choice: \n1 - Private message\n2 - Public chat\n3 - Print all messages\n4 - Quit" << std::endl;
        int UserChoice;
        std::cin >> UserChoice;
        switch (UserChoice) {
        case 1:
            PrivateChat();
            break;
        case 2:
            PublicChat();
            break;
        case 3:
            currentLogin.PrintAllMessages();
            break;
        case 4:
            std::cout << "Bye, bye\n";
            return;
        }
    }
}

bool Chat::CheckPasswordByIndex(std::string userLogin, const std::string& password) {
    
        //проверка по хэшу от пароля
    uint *CheckHash = sha1(password.c_str(), password.size());
    if (table.verify(userLogin, CheckHash)) {
        delete[] CheckHash;
        return true;
    }
    return false;
}

void Chat::PrivateChat() {
    Message message;
    message.from = currentLogin.name;
    std::cout << "Choice users message:\n";

    PrintUsers();

    int userIndex;
    std::cin >> userIndex;
    message.to = users[userIndex].name;
    while (true) {
        std::cout << "Enter a message to " << message.to << " from " << message.from << ": ";
        std::string txt;
       
        std::getline(std::cin, txt);
        message.text = txt;
        users[userIndex].messages.push_back(message);

        if (message.text == "q") {
            return;
        }
    }
}

void Chat::PublicChat() {
    Message message;
    while (true) {
        std::cout << "Write to public chat: ";
        std::getline(std::cin, message.text);
        std::cout << currentLogin.name << ": " << message.text << "\n";
        message.from = currentLogin.login;
        for (int i = 0; i < users.size(); ++i) {
            if (currentLogin.login == users[i].login) {
                continue;
            }
            users[i].messages.push_back(message);
        }
        if (message.text == "q") {
            return;
        }
    }
}