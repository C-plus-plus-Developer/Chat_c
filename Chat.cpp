#include "Chat.h"
int userIndex;
using namespace Mysha;
void Chat::ShowHello() {
    std::cout << "Hello, I am chat" << std::endl;
}

void Chat::Login() {
    std::string userlogin;
    

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
  

    std::cout << "name: ";
    std::string newName;
    std::cin >> newName;

    User newUser;
    newUser.name = newName;
    newUser.login = newLogin;
    //хеширование пароля

    newUser.password = sha1(newPass.c_str(), static_cast <uint>(newPass.size()));
   
    
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
            break;
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
            PrintPrivateMessage(userIndex);
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
   // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка буфера

    if (userIndex < 0 || userIndex >= users.size()) {
        std::cout << "No users. Choise concret user\n";
        PrivateChat();
        return;
    }
    
    std::string reciver = users[userIndex].name;
    message.to = reciver;
    while (true) {
        std::cout << "Enter a message to " << message.to << " from " << message.from << ": ";
        std::string txt;
       
        std::getline(std::cin, txt);
        message.text = txt;

        if (message.text == "q") {
            return;
        }
        users[userIndex].privateMessages.push_back(message);
        users[userIndex].filePrivateMessage = reciver + "_Private.txt";
        savePrivateMessageToFile(users[userIndex].privateMessages, users[userIndex].filePrivateMessage);
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
        publicMessage.push_back(message);
        //запись сообщений в файл
        savePublicMessageToFile(publicMessage, filePublicMessage);
           
    }

}

void Chat::PrintPublicMessage() {
    for (int i = 0; i < publicMessage.size(); ++i) {
        if (publicMessage[i].text == end) continue;
       
         std::cout << publicMessage[i].from << ": " << publicMessage[i].text << "\n";
       
    }
    
}

//работа с файлом Uers.txt
std::vector<User> Chat::loadUsersFromFile(const std::string& filename) {
    std::vector<User> users;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка при открытии файла " << filename << std::endl;
        return users;
    }

    std::string username;
    std::string login;
    while (file >> username>>login)
    {
        User user;
        user.name = username;
        user.login = login;
        user.password = new uint[5]; // Выделяем память для 5 элементов
        for (int i = 0; i < 5; ++i)
        {
            file >> user.password[i];
        }
        users.push_back(user);
    }
    file.close();
    return users;
}

void Chat::saveUsersToFile(const std::vector<User>& users, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка при открытии файла " << filename << std::endl;
        return;
    }

    for (const auto& user : users) { // Используем range-based for loop
        file << user.name << " " << user.login << " ";
        for (int i = 0; i < 5; ++i) {
            file << user.password[i] << " ";
        }
        file << std::endl;
    }
    file.close();
}

//Работа с файлом publicMessage.txt
void Chat::savePublicMessageToFile(const vector<Message>& mess, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка при открытии файла с сообщениями" << filename << std::endl;
        return;
    }
 
    for (const auto& messages : mess) {
        
        file << messages.from << two << messages.text<<end;
    }
   
    file.close();
}

std::vector<Message> Chat::loadPublicMessageFromFile(const std::string& filename) {
    std::vector<Message> mess;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка при открытии файла с сообщениями: " << filename << std::endl;
        return mess;
    }

    std::string line;
    while (std::getline(file, line)) { // Считываем всю строку из файла
        Message message;
        std::stringstream ss(line);  // Создаем строковый поток из строки

        // Извлекаем 'from'
        std::getline(ss, message.from, '#'); // Считываем до первого "#" (начало two)

        //Проверка, чтобы убедиться, что мы действительно получили 'from'
        if (message.from.empty()) continue;

        //Пропускаем 'two'
        std::string temp;
        ss >> temp;


        //Извлекаем 'text' до '/end.\n'
        std::getline(ss, message.text, '/');  // Считываем до первого "/" (начало end)

        //Удаляем лишние пробелы с начала и конца 'text' (Опционально)
        size_t first = message.text.find_first_not_of(" \t\n\r");
        if (std::string::npos != first) message.text = message.text.substr(first);


        mess.push_back(message);
    }

    file.close();
    return mess;
}



//Работа с файлом Private_message.txt
void Chat::savePrivateMessageToFile(const std::vector<Message>& mess, const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cout << "Ошибка при открытии файла с сообщениями" << filename << std::endl;
        return;
    }

    for (const auto& messages : mess) {
        file << messages.from << two << messages.to << two << messages.text << " /end.\n";
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

    std::string line;
    while (std::getline(file, line)) { // Считываем всю строку из файла
        Message message;
        std::stringstream ss(line);  // Создаем строковый поток из строки

        // Извлекаем 'from'
        std::getline(ss, message.from, '#'); // Считываем до первого "#" (начало two)

        //Проверка, чтобы убедиться, что мы действительно получили 'from'
        if (message.from.empty()) continue;

        //Пропускаем 'two'
        std::string temp;
        ss >> temp;

        std::getline(ss, message.to, '#');
        if (message.to.empty()) continue;

        //Пропускаем 'two'

        ss >> temp;
        //Извлекаем 'text' до '/end.\n'
        std::getline(ss, message.text, '/');  // Считываем до первого "/" (начало end)

        //Удаляем лишние пробелы с начала и конца 'text' (Опционально)
        size_t first = message.text.find_first_not_of(" \t\n\r");
        if (std::string::npos != first) message.text = message.text.substr(first);


        mess.push_back(message);
    }


    file.close();
    return mess;
}


void Chat::PrintPrivateMessage(int userIndex) {
    users[userIndex].privateMessages = loadPrivateMessageFromFile(users[userIndex].name + "_Private.txt");

    if (users[userIndex].privateMessages.empty()) {
        std::cout << "No message\n";
    }

    for (int i = 0; i < users[userIndex].privateMessages.size(); ++i) {
        std::cout << "From: " << users[userIndex].privateMessages[i].from << " to: " << users[userIndex].privateMessages[i].to << " text messages: " << users[userIndex].privateMessages[i].text << std::endl;
    }
}