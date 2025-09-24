#include "Chat.h"

using namespace Mysha;
void Chat::ShowHello() {
    std::cout << "Hello, I am chat" << std::endl;
}

void Chat::Login() {
    std::string userlogin;
    int userIndex;

    //users = loadUsersFromFile(fileUser);
    
    if (users.empty()) {
        std::cout << "Users empty. Login to chat user please.\n";
        start();
    }
    std::cout << "Choice concret user" << std::endl;

   
    PrintUsers();

    std::cout << "Write user index: ";
    std::cin >> userIndex;

    if (userIndex < 0 || userIndex >= users.size()) {
        std::cout << "No users. Choise concret user\n";
        Login();
    }

    std::cout << "Write user login: ";
    std::cin >> userlogin;

    std::cout << "Write user password: ";
    std::string pass;
    std::cin >> pass;
    
    if (CheckPasswordByIndex(userIndex, pass)) {
        std::cout << "You have entered to user" << std::endl;
        currentLogin = users[userIndex];
        ShowUserMenu();
    }
    else {
        std::cout << "Wrong password!\n";
        start();
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
    newUser.login = newLogin;
    newUser.password = PassHash;
   
    if (!users.empty()) {
        for (int i = 0; i < users.size(); ++i) {
            if (users[i].name == newName) {
                cout << "Юзер с таким именем уже существует\n";
                AddNewUser();
            }
        }
    }
    users.push_back(newUser);
    //Запись в файл User.txt нового пользователя
    saveUsersToFile(newUser,fileUser);

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
    users = loadUsersFromFile(fileUser);
    
    for (int i = 0; i < users.size(); ++i) {
        std::cout << i << " - " << users[i].name << std::endl;
    }
}

void Chat::ShowUserMenu() {
    while (true) {
        std::cout << "Hi, " << currentLogin.name << std::endl;
        std::cout << "Choice: \n1 - Private message\n2 - Public chat\n3 - Print private messages\n4 - Print public messages\n5 - Quit" << std::endl;
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
            currentLogin.PrintAllMessage();
            break;
        case 4:
            PrintPublicMessage();
            break;

        case 5:
            std::cout << "Bye, bye\n";
            start();
            return;
        }
    }
}
// доработка
bool Chat::CheckPasswordByIndex(int index, const std::string& password) {
    
        //проверка по хэшу от пароля
    uint *CheckHash = sha1(password.c_str(), password.size());

    if (CheckHash == nullptr) std::cout << "CheckHash null\n";
    if (users[index].password == nullptr) std::cout << "pass null\n";
    if (memcmp(CheckHash, (unsigned char*)users[index].password, 20) == 0) {
        delete[] CheckHash;
        return true;
    }
    delete[] CheckHash;
    return false;
}

void Chat::PrivateChat() {
    Message message;
    message.from = currentLogin.name;
    std::cout << "Choice users message:\n";

    PrintUsers();
    
    int userIndex;
    std::cin >> userIndex;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка буфера

    if (userIndex < 0 || userIndex >= users.size()) {
        std::cout << "No users. Choise concret user\n";
        PrivateChat();
    }
    

    message.to = users[userIndex].name;
    while (true) {
        std::cout << "Enter a message to " << message.to << " from " << message.from << ": ";
        std::string txt;
       
        std::getline(std::cin, txt);
        message.text = txt;

        if (message.text == "q") {
            return;
        }
       
        users[userIndex].messages.push_back(message);
        savePrivateMessageToFile(users[userIndex].messages, fileUser);

    }
}

void Chat::PublicChat() {
    Message message;
    while (true) {
        std::cout << "Write to public chat: ";
        std::getline(std::cin, message.text);
        std::cout << currentLogin.name << ": " << message.text << "\n";
        message.from = currentLogin.name;

        if (message.text == "q") {
            return;
        }
         //запись сообщений в файл 
         publicMessage.push_back(message);
         savePublicMessageToFile(publicMessage, filePublicMessage);
           
    }


}

void Chat::PrintPublicMessage() {
    for (int i = 0; i < publicMessage.size(); ++i) {
        std::cout << publicMessage[i].from << ": " << publicMessage[i].text << "\n";
    }
}

void Chat::loadDataFromFile() {
    users = loadUsersFromFile(fileUser);
    publicMessage = loadPublicMessageFromFile(filePublicMessage);
    for (auto& a : users) {
        
    }
}

std::vector<Message> Chat::loadPublicMessageFromFile(const std::string& filename) {
    std::vector<Message> mess;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка при открытии файла с сообщениями" << filename << std::endl;
        return mess;
    }

    Message messages;
    while (file >> messages.from >> messages.to >> messages.text) {
        mess.push_back(messages);
    }

    file.close();
    return mess;
}


std::vector<Message> Chat::loadPrivateMessageFromFile(const std::string& filename) {
    std::vector<Message> mess;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка при открытии файла с сообщениями" << filename << std::endl;
        return mess;
    }

    Message messages;
    while (file >> messages.from >> messages.to >> messages.text) {
        mess.push_back(messages);
    }

    file.close();
    return mess;
}

void Chat::savePrivateMessageToFile(const std::vector<Message>& mess, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка при открытии файла с сообщениями" << filename << std::endl;
        return;
    }

    for (const auto& messages : mess) {
        file << messages.from << " " << messages.to << messages.text << std::endl;
    }

    file.close();
}
void Chat::saveUsersToFile(User user, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла для записи пользователей." << std::endl;
        return;
    }

    file << user.name << ", " << user.login <<", " << user.password << "\n";

    file.close();
}

std::vector<User> Chat::loadUsersFromFile(const std::string& filename) {
    std::vector<User> users;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка при открытии файла с юзерами" << filename << std::endl;
        return users;
    }

    User user;
    while (file >> user.name >> user.login) {
        users.push_back(user);
    }

    file.close();
    return users;
}

void Chat::savePublicMessageToFile(const std::vector<Message>& mess, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка при открытии файла с сообщениями" << filename << std::endl;
        return;
    }

    for (const auto& messages : mess) {
        file << messages.from << " " << messages.to << messages.text << std::endl;
    }

    file.close();
}