#include "Hash_Func.h"


int UserHashTable::hashKey(const std::string& login, int offset) const {
    // ��������� ������
    int sum = 0, i = 0;
    for (; i < strlen(login.c_str()); i++) {
        sum += login[i];
    }
    // ������ �����������
    int f2 = sum % (size * 2);
    // �����
    return (sum % size + offset * f2) % size;
}
// �����������
UserHashTable::UserHashTable(size_t table_size) : size(table_size) {
    table.resize(size);
    // ���������� ��� ������ �����, password_hash ����� nullptr
    for (auto& user : table) {
        user.login = "";
        user.password_hash = nullptr;
    }
}

// ����������
UserHashTable::~UserHashTable() {
    for (auto& user : table) {
        delete[] user.password_hash;
    }
}

// ����� ���������� ������������
void UserHashTable::add(const std::string& login, const uint* hashVal) {
    // �������� ������������� � ����������, ��������:
    int index = -1;
    size_t start_idx = index;
    size_t count = 0;

    for (int i = 0;;i++) {
        index = hashKey(login, i);
        if (table[index].empty()) {
            // ��������� ������
            table[index].login = login;
            table[index].password_hash = new uint[5];
            for (int i = 0; i < 5; ++i) {
                table[index].password_hash[i] = hashVal[i];
            }
            return;
        }
        if (table[index].login == login) {
            // ��������� ������
            delete[] table[index].password_hash;
            table[index].password_hash = new uint[5];
            for (int i = 0; i < 5; ++i) {
                table[index].password_hash[i] = hashVal[i];
            }
            return;
        }
        index = (index + 1) % size;
        ++count;
        // ���� ���� �������� � ��������� ������� ��� �������� �������:
        if (count >= size) {
            // ������� ������, ����� ����������
            resize();
            // ����� ���������� ����� ������� �������
            add(login, hashVal);
            return;
        }
    }
}

// ����� �������� ������������
bool UserHashTable::verify(const std::string& login, const uint* hashVal) {
    int index = -1;
   

    for (int i = 0;i<size;i++) {
        index = hashKey(login, i);
        if (table[index].empty()) {
            std::cout << "������������ �� ����������.\n";
            return false; // ������������ �� ������
        }
        if (table[index].login == login) {
            // �����, ���������� ��� ������
            for (int i = 0; i < 5; ++i) {
                if (table[index].password_hash[i] != hashVal[i]) {
                    std::cout << "�������� ������!\n";
                    return false; // ������ �� ���������
                }
            }
            return true; // ������������ ������ � ������ ���������
        }
        index = (index + 1) % size;
    }
    return false; // ������������ �� ������
}

void UserHashTable::resize() {
    size_t new_size = size * 2; // ����������� ������ �������
    std::vector<UserEntry> new_table(new_size);

    // ������� ����� ������ � �������������� ��� ������� ��������
    for (auto& user : new_table) {
        user.login = "";
        user.password_hash = nullptr;
    }

    // ��������� �������, ����� ��������� �������������
    std::vector<UserEntry> old_table = std::move(table);
    size = new_size;
    table = std::move(new_table);

    // ��������� ������������� �� ������ ������� � �����
    for (const auto& user : old_table) {
        if (!user.login.empty()) {
            // ���������� ������������ ����� add, ����� �������� ������������
            add(user.login, user.password_hash);
        }
    }
}