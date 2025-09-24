#pragma once
#include <iostream>
#include <string>
#include <fstream>

class Message {
public:
    std::string text;
    std::string to;
    std::string from;

    friend std::ostream& operator<<(std::ostream& os, const Message& mess) {
        os << "From: " << mess.from << ", to: " << mess.to<<", text: "<<mess.text<<"\n";
        return os;
    }
};

