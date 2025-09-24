#include "User.h"

void User::PrintAllMessage() {

    if (messages.empty()) {
        std::cout << "No message\n";
    }

    for (int i = 0; i < messages.size(); ++i) {
        std::cout << "From: " << messages[i].from << ": " << messages[i].text << std::endl;
    }

    
}
