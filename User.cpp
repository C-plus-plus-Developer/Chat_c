#include "User.h"
#include <iostream>

void User::PrintAllMessages() {
    for (int i = 0; i < messages.size(); ++i) {
        std::cout << "From: " << messages[i].from << ": " << messages[i].text << std::endl;
    }
}