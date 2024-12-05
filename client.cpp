#include <iostream>
#include <string>      // For strlen
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

void log(const std::string& message) {
    std::cout << "[LOG]" << message << std::endl;
}

int main() {

    log("Connected to the server");

    WSADATA wsadata;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in server_addr;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9090); // The same port as the server
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection to server failed.\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to the server\n";

    // Communication Loop
    std::string message;
    char buffer[1024] = {0};

    while(true) {
        std::cout << "Enter message (type 'exit' to quit): ";
        std::getline(std::cin, message);

        send(sock, message.c_str(), message.length(), 0);

        if(message.empty()) {
            std::cout << "Message cannot be empty. Please enter a valid message";
            continue;
        }

        if (send(sock, message.c_str(), message.length(), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        }

        if (message == "exit") {
            break;
        }

        int byteReceived = recv(sock, buffer, 1024, 0);
        if (byteReceived > 0) {
            buffer[byteReceived] = '\0';
            std::cout << "Server: " << buffer << std::endl;
        } else {
            std::cerr << "Failed to receive message from server. Error" << WSAGetLastError() << std::endl;
            break;
        }
    }

    // Clean up
    closesocket(sock);
    WSACleanup();
    log("Client socket closed and resources cleaned up.");

    return 0;
}
