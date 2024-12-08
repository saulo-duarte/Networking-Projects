#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8181

int main() {
    int s, c;
    socklen_t addrlen;
    struct sockaddr_in srv, cli;
    char buf[512];
    char *data;

    // Zero out the socket address structures
    memset(&srv, 0, sizeof(srv));
    memset(&cli, 0, sizeof(cli));

    // Create socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Failed to create socket");
        return -1;
    }

    // Configure server address
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = INADDR_ANY;  // Listen on all available interfaces
    srv.sin_port = htons(PORT);

    // Bind socket
    if (bind(s, (struct sockaddr *)&srv, sizeof(srv)) < 0) {
        perror("Failed to bind socket");
        close(s);
        return -1;
    }

    // Listen for connections
    if (listen(s, 5) < 0) {
        perror("Failed to listen");
        close(s);
        return -1;
    }

    printf("Listening on 0.0.0.0: %d\n", PORT);

    // Accept connection
    addrlen = sizeof(cli);
    c = accept(s, (struct sockaddr *)&cli, &addrlen);
    if (c < 0) {
        perror("Failed to accept connection");
        close(s);
        return -1;
    }

    printf("Client Connected\n");

    // Read from client
    ssize_t bytes_read = read(c, buf, sizeof(buf) - 1);
    if (bytes_read > 0) {
        buf[bytes_read] = '\0';  // Null-terminate the received data
        printf("Received: %s", buf);
    }

    // Send response
    data = "httpd v1.0\n";
    write(c, data, strlen(data));

    // Close connections
    close(c);
    close(s);

    return 0;
}