#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_service io_service;

        // Создание TCP-сокета
        tcp::socket socket(io_service);
        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8080));

        // Отправка HTTP-запроса
        std::string request = "POST /server/control/exit HTTP/1.1\r\n\r\n";
        boost::asio::write(socket, boost::asio::buffer(request));

        // Чтение HTTP-ответа
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, "\r\n");
        std::string response = boost::asio::buffer_cast<const char*>(buffer.data());
        buffer.consume(buffer.size());

        std::cout << "Response: " << response << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}