#include <iostream>
#include <list>
#include <cstring>
#include <iomanip>
#include <Windows.h>
using namespace std;

const int hashGroups = 11;

struct Node {
    int key;
    string value;
};

class HashTable {
private:
    list<Node> table[hashGroups];

public:
    bool isEmpty() const;
    int hashFunc(int key);
    void insertItem(int key, string value);
    void removeItem(int key);
    string searchTable(int key);
    void printTable();
};

bool HashTable::isEmpty() const {
    int sum = 0;
    for (int i = 0; i < hashGroups; i++) {
        sum += table[i].size();
    }

    return sum == 0;
}

int HashTable::hashFunc(int key) {
    return key % hashGroups;
}

void HashTable::insertItem(int key, string value) {
    int hashValue = hashFunc(key);
    table[hashValue].push_back({ key, value });
}

void HashTable::removeItem(int key) {
    int hashValue = hashFunc(key);
    auto& cell = table[hashValue];
    auto itr = cell.begin();

    for (; itr != cell.end(); itr++) {
        if (itr->key == key) {
            cell.erase(itr);
            break;
        }
    }
}

void HashTable::printTable() {
    for (int i = 0; i < hashGroups; i++) {
        if (table[i].empty()) {
            continue;
        }

        cout << "Hash Group " << i << ":" << endl;

        for (const auto& node : table[i]) {
            cout << "Key: " << setw(5) << node.key << " | Value: " << node.value << endl;
        }
    }
}

string HashTable::searchTable(int key) {
    int hashValue = hashFunc(key);
    auto& cell = table[hashValue];

    for (const auto& node : cell) {
        if (node.key == key) {
            return node.value;
        }
    }

    return "";
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    HashTable ht;
    int choice, key;
    string value;

    while (true) {
        system("cls");
        cout << "===================================" << endl;
        cout << "        Меню Хеш-таблиці          " << endl;
        cout << "===================================" << endl;
        cout << "1. Перевірити, чи хеш-таблиця порожня." << endl;
        cout << "2. Додати елемент до хеш-таблиці." << endl;
        cout << "3. Видалити елемент з хеш-таблиці." << endl;
        cout << "4. Вивести всю хеш-таблицю." << endl;
        cout << "5. Пошук у хеш-таблиці." << endl;
        cout << "0. Вийти." << endl;
        cout << "===================================" << endl;
        cout << "Виберіть пункт: ";
        cin >> choice;
        cout << "===================================" << endl;

        switch (choice) {
            case 0:
                return 0;
            case 1:
                if (ht.isEmpty()) {
                    cout << "Хеш-таблиця порожня." << endl;
                } else {
                    cout << "Хеш-таблиця не є порожньою." << endl;
                }
                break;
            case 2:
                cout << "Введіть ключ: ";
                cin >> key;
                cout << "Введіть значення: ";
                cin >> value;
                ht.insertItem(key, value);
                break;
            case 3:
                cout << "Введіть ключ для видалення: ";
                cin >> key;
                ht.removeItem(key);
                break;
            case 4:
                ht.printTable();
                break;
            case 5:
                cout << "Введіть ключ для пошуку: ";
                cin >> key;
                value = ht.searchTable(key);
                if (value.empty()) {
                    cout << "Значення не знайдено." << endl;
                } else {
                    cout << "Знайдене значення: " << value << endl;
                }
                break;
            default:
                cout << "Невірний вибір. Спробуйте ще раз." << endl;
        }

        system("pause");
    }

    return 0;
}
