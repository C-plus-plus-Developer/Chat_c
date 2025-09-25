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

    if (!users.empty()) {
        for (int i = 0; i < users.size(); ++i) {
            if (users[i].login == newLogin) {
                cout << "Юзер с таким логином уже существует\n";
                AddNewUser();
                return;
            }
        }
    }

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
   
    
    users.push_back(newUser);
    //Запись в файл User.txt нового пользователя
    saveUsersToFile(users,fileUser);

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

bool Chat::CheckPasswordByIndex(int index, const std::string& password) {
    
        //проверка по хэшу от пароля
    uint *CheckHash = sha1(password.c_str(), password.size());
    bool match = memcmp(CheckHash, users[index].password, 20) == 0;

    // Освобождаем память, занятую под вычисленный хэш
    delete[] CheckHash;

    return match;
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

        if (message.text == "q") {
            return;
        }

        std::cout << currentLogin.name << ": " << message.text << "\n";
        message.from = currentLogin.name;

        //запись сообщений в файл
         savePublicMessageToFile(message, filePublicMessage);
           
    }


}

void Chat::PrintPublicMessage() {
    for (int i = 0; i < publicMessage.size(); ++i) {
        std::cout << publicMessage[i].from << ": " << publicMessage[i].text << "\n";
    }
}
/*
void Chat::loadDataFromFile() {
    users = loadUsersFromFile(fileUser);
    publicMessage = loadPublicMessageFromFile(filePublicMessage);
    for (auto& a : users) {
        
    }
}
*/

void Chat::savePrivateMessageToFile(const std::vector<Message>& mess, const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cout << "Ошибка при открытии файла с сообщениями" << filename << std::endl;
        return;
    }

    for (const auto& messages : mess) {
        file << messages.from << " " << messages.to << messages.text << std::endl;
    }

    file.close();
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


//работа с файлом Uers.txt
void Chat::saveUsersToFile(const std::vector<User>& users, const std::string& filename) {
    std::ofstream file(filename, std::ios::trunc | std::ios::binary); // Truncate existing file
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла для записи пользователей." << std::endl;
        return;
    }

    for (const auto& user : users) {
        file.write(user.name.c_str(), user.name.size() + 1); // write null terminated string
        file.write(user.login.c_str(), user.login.size() + 1);
        file.write(reinterpret_cast<const char*>(user.password), 20); // Write raw bytes of hash
    }

    file.close();
}
std::vector<User> Chat::loadUsersFromFile(const std::string& filename) {
    std::vector<User> users;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Ошибка при открытии файла с юзерами" << filename << std::endl;
        return users;
    }

    while (file.peek() != EOF) {
        User user;
        char buffer[256];
        file.read(buffer, sizeof(buffer)); // Read null-terminated strings
        user.name = buffer;
        file.read(buffer, sizeof(buffer));
        user.login = buffer;
        uint passwordHash[5]; // assuming 5 elements as per your example
        file.read(reinterpret_cast<char*>(passwordHash), 20); // read raw hash bytes
        user.password = new uint[5]{ passwordHash[0], passwordHash[1], passwordHash[2], passwordHash[3], passwordHash[4] };
        users.push_back(user);
    }

    file.close();
    return users;
}
//Работа с файлом publicMessage.txt


void Chat::savePublicMessageToFile(Message mess, const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cout << "Ошибка при открытии файла с сообщениями" << filename << std::endl;
        return;
    }

     file << mess.from << " "  << mess.text << std::endl;
    

    file.close();
}

std::vector<Message> Chat::loadPublicMessageFromFile(const std::string& filename) {
    std::vector<Message> mess;
    std::ifstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cout << "Ошибка при открытии файла с сообщениями" << filename << std::endl;
        return mess;
    }

    Message messages;
    while (file >> messages.from >> messages.text) {
        mess.push_back(messages);
    }

    file.close();
    return mess;
}
