#include "Hash_Func.h"


int UserHashTable::hashKey(const std::string& login, int offset) const {
    // вычисл€ем индекс
    int sum = 0, i = 0;
    for (; i < strlen(login.c_str()); i++) {
        sum += login[i];
    }
    // второе хеширование
    int f2 = sum % (size * 2);
    // проба
    return (sum % size + offset * f2) % size;
}
//  онструктор
UserHashTable::UserHashTable(size_t table_size) : size(table_size) {
    table.resize(size);
    // »значально все логины пусты, password_hash равен nullptr
    for (auto& user : table) {
        user.login = "";
        user.password_hash = nullptr;
    }
}

// ƒеструктор
UserHashTable::~UserHashTable() {
    for (auto& user : table) {
        delete[] user.password_hash;
    }
}

// ћетод добавлени€ пользовател€
void UserHashTable::add(const std::string& login, const uint* hashVal) {
    // ѕроверка заполненности и расширение, например:
    int index = -1;
    size_t start_idx = index;
    size_t count = 0;

    for (int i = 0;;i++) {
        index = hashKey(login, i);
        if (table[index].empty()) {
            // свободна€ €чейка
            table[index].login = login;
            table[index].password_hash = new uint[5];
            for (int i = 0; i < 5; ++i) {
                table[index].password_hash[i] = hashVal[i];
            }
            return;
        }
        if (table[index].login == login) {
            // обновл€ем пароль
            delete[] table[index].password_hash;
            table[index].password_hash = new uint[5];
            for (int i = 0; i < 5; ++i) {
                table[index].password_hash[i] = hashVal[i];
            }
            return;
        }
        index = (index + 1) % size;
        ++count;
        // ≈сли цикл вернулс€ к стартовой позиции без успешной вставки:
        if (count >= size) {
            // таблица полна€, нужно расширение
            resize();
            // ѕосле расширени€ снова попытка вставки
            add(login, hashVal);
            return;
        }
    }
}

// ћетод проверки пользовател€
bool UserHashTable::verify(const std::string& login, const uint* hashVal) {
    int index = -1;
   

    for (int i = 0;i<size;i++) {
        index = hashKey(login, i);
        if (table[index].empty()) {
            std::cout << "ѕользователь не существует.\n";
            return false; // пользователь не найден
        }
        if (table[index].login == login) {
            // нашли, сравниваем хэш парол€
            for (int i = 0; i < 5; ++i) {
                if (table[index].password_hash[i] != hashVal[i]) {
                    std::cout << "Ќеверный пароль!\n";
                    return false; // пароль не совпадает
                }
            }
            return true; // пользователь найден и пароль совпадает
        }
        index = (index + 1) % size;
    }
    return false; // пользователь не найден
}

void UserHashTable::resize() {
    size_t new_size = size * 2; // увеличиваем размер таблицы
    std::vector<UserEntry> new_table(new_size);

    // —оздаем новый вектор и инициализируем его пустыми запис€ми
    for (auto& user : new_table) {
        user.login = "";
        user.password_hash = nullptr;
    }

    // ¬ременна€ таблица, чтобы добавл€ть пользователей
    std::vector<UserEntry> old_table = std::move(table);
    size = new_size;
    table = std::move(new_table);

    // ѕереносим пользователей из старой таблицы в новую
    for (const auto& user : old_table) {
        if (!user.login.empty()) {
            // »спользуем существующий метод add, чтобы вставить пользовател€
            add(user.login, user.password_hash);
        }
    }
}