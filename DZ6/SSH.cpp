#include <iostream>
#include <string>
#include <sstream>
#include <openssl/sha.h> // использование SHA256 хэш-функции

using namespace std;

// Функция, возвращающая SHA256 хэш от строки
string sha256(string str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << hex << (int)hash[i];
    }
    return ss.str();
}

// Функция для регистрации нового клиента
void registerClient(string login, string password) {
    // Зашифровываем пароль клиента с помощью SHA256 хэш-функции
    string hashedPassword = sha256(password);
    // Здесь должна быть логика сохранения пароля и логина в базе данных или файле
    // Например, можно сохранить их в текстовом файле в формате "login:hashedPassword"
    cout << "Клиент с логином " << login << " зарегистрирован.\n";
}

// Функция для проверки логина и пароля клиента при входе на сервер
bool checkClient(string login, string password) {
    // Получаем хэш пароля, который был сохранен при регистрации
    string savedPasswordHash;
    
    if (savedPasswordHash.empty()) {
        return false; // Если клиент с таким логином не зарегистрирован
    }
    // Получаем хэш пароля, введенного клиентом при входе на сервер
    string enteredPasswordHash = sha256(password);
    // Сравниваем хэши паролей
    return savedPasswordHash == enteredPasswordHash;
}

int main() {
    // Пример использования функций регистрации и проверки клиента
    registerClient("user1", "password1");
    registerClient("user2", "password2");
    string login, password;
    cout << "Введите логин и пароль для входа на сервер: ";
    cin >> login >> password;
    if (checkClient(login, password)) {
        cout << "Добро пожаловать, " << login << "!\n";
    } else {
        cout << "Неверный логин или пароль.\n";
    }
    return 0;
}