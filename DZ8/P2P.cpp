#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <pthread.h>

using boost::asio::ip::tcp;

const int max_message_length = 1024;

// Структура для хранения информации о клиенте
struct client_info {
    tcp::socket socket;
    std::string id;
    std::string name;
};

// Функция обработки соединения с клиентом
void handle_client(client_info* client, std::vector<client_info*>& clients) {
    try {
        char message[max_message_length];
        while (true) {
            boost::asio::read(client->socket, boost::asio::buffer(message, max_message_length));
            std::cout << "[" << client->name << "]: " << message << std::endl;
            for (auto& other_client : clients) {
                if (other_client != client) {
                    boost::asio::write(other_client->socket, boost::asio::buffer(message, max_message_length));
                }
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << std::endl;
    }
}

int main() {
    try {
        boost::asio::io_service io_service;

        // Создание сервера
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));

        std::vector<client_info*> clients;

        while (true) {
            // Ожидание подключения нового клиента
            client_info* client = new client_info;
            acceptor.accept(client->socket);

            // Чтение информации об идентификаторе и имени клиента
            char id[max_message_length];
            char name[max_message_length];
            boost::asio::read(client->socket, boost::asio::buffer(id, max_message_length));
            boost::asio::read(client->socket, boost::asio::buffer(name, max_message_length));
            client->id = id;
            client->name = name;

            // Добавление клиента в список
            clients.push_back(client);

            // Создание потока для обработки соединения с клиентом
            pthread_t thread;
            pthread_create(&thread, NULL, (void* (*)(void*))handle_client, client);
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
