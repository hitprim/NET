#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#include <socket_wrapper/socket_headers.h>
#include <socket_wrapper/socket_wrapper.h>
#include <socket_wrapper/socket_class.h>


int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <host> <port>" << std::endl;
        return EXIT_FAILURE;
    }

    const int port { std::stoi(argv[2]) };
    const std::string host_name = { argv[1] };

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        std::cerr << "Faild to init Winsock: " << result << std::endl;
        return EXIT_FAILURE;
    }

    SOCKET sock = socket(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET)
    {
        std::cerr << "Faild to create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return EXIT_FAILURE;
    }

    const char* servName = host_name.c_str();
    struct addrinfo hints = { 0 };
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo* addrInfo = { 0 };
    result = getaddrinfo(servName, reinterpret_cast<PCSTR> (port), &hints, &addrInfo);
    if (result != 0)
    {
        std::cerr << "Name of server error: " << result << std::endl;
        WSACleanup();
        return EXIT_FAILURE;
    }
    
    for (struct addrinfo* ptr = addrInfo; ptr != nullptr; ptr = ptr->ai_next)
    {
        result = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
        if(result == SOCKET_ERROR)
        {
            continue;
        }
        break;
    }

    if (result == SOCKET_ERROR)
    {
        std::cerr << "Failed to connect to server: " << WSAGetLastError() << std::endl;
        freeaddrinfo(addrInfo);
        closesocket(sock);
        WSACleanup();
        return EXIT_FAILURE;
    }

    const char* messeage = "Test mess";
    result = send(sock, messeage, strlen(messeage), 0);
    if (result == SOCKET_ERROR)
    {
        std::cerr << "Failed to send data: " << WSAGetLastError() << std::endl;
        freeaddrinfo(addrInfo);
        closesocket(sock);
        WSACleanup();
        return EXIT_FAILURE;
    }

    char buffer[1024];
    int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
    if (bytesReceived == SOCKET_ERROR)
    {
        std::cerr << "Failed to get data: " << WSAGetLastError() << std::endl;
        freeaddrinfo(addrInfo);
        closesocket(sock);
        WSACleanup();
        return EXIT_FAILURE;
    }

    buffer[bytesReceived] = '\0';
    printf("Server answer: %c\n", buffer);

    freeaddrinfo(addrInfo);
    closesocket(sock);
    WSACleanup();

    return EXIT_SUCCESS;
}