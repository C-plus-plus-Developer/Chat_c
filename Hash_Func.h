#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "sha1.h"
#include <stddef.h>

using namespace Mysha;

struct UserEntry {
    std::string login;
    uint* password_hash; // массив из 5 uint

    bool empty() {
        return login.empty() && password_hash == nullptr;
    }
};

class UserHashTable {
private:
    std::vector<UserEntry> table;
    size_t size;

    int hashKey(const std::string& login, int offset) const;

public:
    UserHashTable(size_t table_size = 100);
    ~UserHashTable();


    void add(const std::string& login, const uint* hashVal);
    void resize();
    bool verify(const std::string& login, const uint* hashVal);
};