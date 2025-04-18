#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void interactive_mode(int sock) {
    char buffer[256];
    while (1) {
        printf("Enter a number: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(sock, buffer, strlen(buffer), 0);
        int received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (received > 0) {
            buffer[received] = '\0';
            printf("Server: %s\n", buffer);
            if (strstr(buffer, "Correct!")) break;
        }
    }
}

void automatic_mode(int sock) {
    char buffer[256];
    int low = 1, high = 100, guess;
    while (1) {
        guess = (low + high) / 2;
        snprintf(buffer, sizeof(buffer), "%d", guess);
        send(sock, buffer, strlen(buffer), 0);
        int received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (received > 0) {
            buffer[received] = '\0';
            printf("Server: %s\n", buffer);
            if (strstr(buffer, "Correct!")) break;
            if (strstr(buffer, "Higher")) low = guess + 1;
            else high = guess - 1;
        }
    }
}

int main() {
    char server_ip[16];
    int port;

    printf("Enter server IP: ");
    scanf("%15s", server_ip);
    printf("Enter server port: ");
    scanf("%d", &port);
    getchar();

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    printf("Connected to server\n");
    printf("Select mode: 1 - Interactive, 2 - Automatic\n");
    int mode;
    scanf("%d", &mode);
    getchar();

    if (mode == 1) interactive_mode(sock);
    else automatic_mode(sock);
    
    close(sock);
    return 0;
}
