#include <iostream>
#include <string>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <csignal>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 9090

SOCKET server_fd; 

void signalHandler(int signum) {
    std::cout << "Shutting down server...." << std::endl;
    if (server_fd != INVALID_SOCKET) {
        closesocket(server_fd); 
    }
    WSACleanup(); 
    exit(signum); 
}

void log(const std::string& message) {
    std::cout << "[LOG] " << message << std::endl;
}


void handle_client(SOCKET client_socket) {
    char buffer[1024] = {0}; 
    const char* hello = "Hello from server"; 
    int valread;

    while (true) {
       
        valread = recv(client_socket, buffer, sizeof(buffer), 0);
        if (valread > 0) {
            buffer[valread] = '\0'; 
            std::cout << "Received: " << buffer << std::endl;

            
            if (strcmp(buffer, "exit") == 0) {
                std::cout << "Client disconnected." << std::endl;
                break;
            }

            
            if (send(client_socket, hello, strlen(hello), 0) == SOCKET_ERROR) {
                std::cerr << "Send failed: " << WSAGetLastError() << std::endl; 
            } else {
                std::cout << "Message sent" << std::endl;
            }
        } else if (valread == 0) {
            std::cout << "Client disconnected." << std::endl;
            break; 
        } else {
            std::cerr << "Error receiving message" << std::endl;
            break;
        }
    }

   
    closesocket(client_socket);
}

int main() {
    signal(SIGINT, signalHandler); 

    log("Server is starting up....");
    
    WSADATA wsadata;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        SOCKET new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (new_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &address.sin_addr, ip, sizeof(ip)); 
        std::cout << "New connection established from " << ip << std::endl;
        log("New connection established from " + std::string(ip));

        std::thread client_thread(handle_client, new_socket);
        client_thread.detach();
    }

    closesocket(server_fd);
    WSACleanup();

    return 0;
}
