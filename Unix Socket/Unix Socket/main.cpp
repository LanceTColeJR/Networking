//
//  main.cpp
//  Unix Socket
//
//  Created by Lance Cole on 2/10/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

#define BUFFER_SIZE 1024

void handle_request(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_read;
    
    // Read request until double newline
    while ((bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        if (strstr(buffer, "\r\n\r\n")) {
            break;
        }
    }

    // Parse GET line
    char *get_line = strtok(buffer, "\r\n");
    if (strncmp(get_line, "GET ", 4) != 0) {
        // Not a GET request, send error
        send(client_socket, "HTTP/1.1 400 Bad Request\r\n\r\n", 28, 0);
        return;
    }
    
    char *resource = strtok(NULL, " ");
    if (strcmp(resource, "/") == 0) {
        resource = "index.html"; // Default to index.html
    }
    
    // Remove leading slashes and dots for security
    while (*resource == '/' || *resource == '.') {
        resource++;
    }
    
    // Open requested file
    FILE *file = fopen(resource, "rb");
    if (file == NULL) {
        // File not found, send 404
        send(client_socket, "HTTP/1.1 404 Not Found\r\n\r\n<b>404 Error - resource not found on this server</b>", 59, 0);
        return;
    }
    
    // Send 200 OK response
    send(client_socket, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n", 43, 0);

    // Read and send file contents
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        send(client_socket, buffer, bytes_read, 0);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);
    int server_socket;
    struct sockaddr_in server_addr;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket creation failed");
        exit(1);
    }
    
    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(1);
    }
    
    // Listen for connections
    if (listen(server_socket, 5) < 0) {
        perror("listen failed");
        exit(1);
    }

    printf("Server listening on port %d\n", port);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }
        
        handle_request(client_socket);
        close(client_socket);
    }

    close(server_socket);
    return 0;
}
