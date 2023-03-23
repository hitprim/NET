#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <vector>

using boost::asio::ip::tcp;

void session(tcp::socket sock) {
  try {
    std::array<char, 1024> data;
    while (true) {
      boost::system::error_code error;
      size_t length = sock.read_some(boost::asio::buffer(data), error);
      if (error == boost::asio::error::eof)
        break;  // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error);  // Some other error.

      std::string message(data.data(), length);
      std::cout << "Received: " << message << std::endl;

      std::string response = "Hello from server!";
      boost::asio::write(sock, boost::asio::buffer(response));
    }
  } catch (std::exception& e) {
    std::cerr << "Exception in thread: " << e.what() << std::endl;
  }
}

int main() {
  try {
    boost::asio::io_service io_service;

    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));
    std::cout << "Server started, listening on port 1234" << std::endl;

    std::vector<std::thread> threads;
    for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
      threads.emplace_back([&io_service]() { io_service.run(); });
    }

    while (true) {
      tcp::socket socket(io_service);
      acceptor.accept(socket);
      io_service.post(std::bind(session, std::move(socket)));
    }
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  return 0;
}