#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char buffer[BUFSIZ];

int main(int argc, char *argv[]) {
    int client_socket, len;
    struct sockaddr_in server_addr;

    // Check for sanity
    if (argc != 3) {
        printf("usage: c IP_ADDRESS PORT_NUMBER\n");
        return 1;
    }

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("grrr, can't get a client socket!\n");
        return 3;
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));

    // Attempt a connection to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("grrr, can't connect to server!\n");
        close(client_socket);
        return 4;
    }

    // Loop to keep client running
    while (1) {
        printf("Enter a command [date | who | df | exit]: ");
        fflush(stdout);
     //      printf("Enter a command [date | who | df | exit]: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';

        // Check for exit command
        if (strcmp(buffer, "exit") == 0) {
            break; // Exit the loop
        }

        // Send command to server
        write(client_socket, buffer, strlen(buffer));

        // Receive response from server
        len = read(client_socket, buffer, sizeof(buffer));
        buffer[len] = '\0'; // Null-terminate the response
        printf("Result of command:\n%s\n\n", buffer);
    }

    // Cleanup
    close(client_socket);
    printf("Client is finished\n");

    return 0;
}


