#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_IP "192.168.1.226" //change this if working from home
#define SERVER_PORT 8080
#define TIMEOUT 10
#define TIMEOUT_MSG "Keep_Alive"

char buffer[BUFSIZ];

void send_msg(int client_socket) {
	write(client_socket, TIMEOUT_MSG, strlen(TIMEOUT_MSG));
}


int main(int argc, char *argv[]) {
    int client_socket, len;
    struct sockaddr_in server_addr;
    time_t last_command_time = time(NULL);
    	
	
    // Check for sanity
   


    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("grrr, can't get a client socket!\n");
        return 3;
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // Attempt a connection to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("grrr, can't connect to server!\n");
        close(client_socket);
        return 4;
    }


    while(1) {
    	
    	
    	if (difftime(time(NULL), last_command_time) >= TIMEOUT) {
    		send_msg(client_socket);
    		last_command_time = time(NULL);
    	}
    	
        printf("Enter a command [ stream | W | A | D | S | exit ]: ");
        fflush(stdout);
     
     	//printf("Enter a command [date | who | df | exit]: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[strlen(buffer) - 1] == '\n') {
            buffer[strlen(buffer) - 1] = '\0';
	}
        // Check for exit command
        if (strcmp(buffer, "exit") == 0) {
            break; // Exit the loop
        }

        // Send command to server
        write(client_socket, buffer, strlen(buffer));

        // Receive response from server
        len = read(client_socket, buffer, sizeof(buffer));
        buffer[len] = '\0'; // Null-terminate the response
        
        if (strcmp(buffer, "Keep_Alive") !=0) {
        printf("Result of command:%s\n\n", buffer);
       }
      	last_command_time = time(NULL);
      }  
    // Cleanup
    close(client_socket);
    printf("Client is finished\n");
    return 0;
}

