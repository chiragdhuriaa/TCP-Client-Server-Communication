# TCP-Client-Server-Communication

Overview
This project demonstrates a simple implementation of a TCP-based client-server communication system using C++ and the Winsock library on Windows. The project covers the fundamentals of socket programming, multi-client support using threads, and proper resource management, providing a foundation for building more advanced networked applications.

Key Features

    1. Client-Server Architecture:
    A basic TCP client connects to a server, allowing message exchanges. The server responds to every message received from the client.
    2. Multithreaded Server:
    The server can handle multiple clients simultaneously by spawning separate threads for each incoming connection.
    3. Graceful Shutdown:
    The server supports a graceful shutdown mechanism triggered by an interrupt signal (e.g., pressing Ctrl+C), which ensures all resources are cleaned up properly.
    4. Message Communication:
    The client sends messages to the server, and the server responds with a fixed message ("Hello from server"). Clients can close the connection by sending the "exit" command.

How It Works
Server

    Listens for incoming connections on port 9090.
    Upon receiving a connection, it creates a new thread to handle communication with that client.
    The server continuously listens for messages from the client and sends back a response.
    If the client sends "exit", the server closes the client’s connection.

Client

    Connects to the server running at 127.0.0.1 (localhost) on port 9090.
    The client sends messages to the server, waits for responses, and displays them.
    The client can disconnect by typing "exit".

Technologies Used

    1. C++: The core programming language for both the server and client.
    2. Winsock: Windows API for managing TCP/IP communication.
    3. Multithreading: Utilized in the server to allow simultaneous communication with multiple clients.

Future Enhancements

    1. Message Parsing: Develop functionality to handle different types of messages (e.g., commands, queries).
    2. Security: Add encryption for secure communication between the client and server.
    3. GUI Support: Build a graphical user interface for easier interaction with the server and client.
    4. Scalability: Implement a more scalable server architecture, such as using thread pools or non-blocking sockets.
