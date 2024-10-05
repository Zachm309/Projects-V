
 //server.c
 
 //This is a sample internet server application that will respond
 //to requests on reconfigurable port 
 
 //Almost reciving data from python. Check buf SIZE

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>



#define PORT 5000
#define PYTHON_BUFFER 256

char buffer[BUFSIZ];
char python_output[PYTHON_BUFFER];


/*
 * this signal handler is used to catch the termination
 * of the child. Needed so that we can avoid wasting
 * system resources when "zombie" processes are created
 * upon exit of the child (as the parent could concievably
 * wait for the child to exit)
 */

void SigCatcher (int n)
{
    wait3 (NULL, WNOHANG, NULL);    
	signal (SIGCHLD, SigCatcher);
}

int serial(const char *message) {
	
	int serial_port = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (serial_port < 0) {
		printf("Error %i from open: %s\n", errno, strerror(errno));
		return -1;
	}
	
	struct termios tty;
	memset(&tty, 0, sizeof(tty));
	
	if (tcgetattr(serial_port, &tty) != 0) {
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
		return -1;
	}
	
	//settint flag
	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);
	
	tty.c_cflag &= ~PARENB; // NO parity
	tty.c_cflag &= ~CSTOPB; // 1 bit stop
	tty.c_cflag &= ~CSIZE; // Clear the size mask
	tty.c_cflag |= ~CS8; // No parity
	
	tty.c_cflag &= ~CRTSCTS;
	tty.c_cflag |= CREAD | CLOCAL;
	
	tty.c_lflag &= ~ICANON; //raw mode
	tty.c_lflag &= ~(ECHO | ECHOE | ISIG);
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // no software flow control
	tty.c_oflag &= ~OPOST; //Raw 
	
	
	tty.c_cc[VMIN] = 0;
	tty.c_cc[VTIME] = 10; // 1 second time
	
	
	
	//
	
	if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
		printf("Error %i from tcsetattr: %s\n");
		return -1;
	}
	
	int n_written = write(serial_port, message, strlen(message));
	if (n_written < 0) {
		printf("Error %i from write: %s\n", errno, strerror(errno));
	}
	
	close(serial_port);
	return 0;
}



int main (int argc, char *argv[])
{
        //my own server socket entity and alsso the client enitty im talking to
	int server_socket, client_socket;
	int client_len;
	struct sockaddr_in client_addr, server_addr; //tracks client and my own IP address
	int len, i;
	FILE *p;
        // ./server 8080
	if (argc != 2) {
		printf ("usage: c PORT_NUMBER\n");
		return 1;
	}	/* endif */
        //init 
	/*
	 * install a signal handler for SIGCHILD (sent when the child terminates)
	 */

	signal (SIGCHLD, SigCatcher);

	/*
	 * obtain a socket for the server
	 */
//1. Create socket entity
	if ((server_socket = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		printf ("grrr, can't get the server socket\n");
		return 1;
	}	/* endif */

	/*
	 * initialize our server address info for binding purposes
	 */
// Fill in the server information for comm
	memset (&server_addr, 0, sizeof (server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl (INADDR_ANY); // the default IP of my computer 
	server_addr.sin_port = htons (atoi(argv[1])); // "8080" --> 8080 (big endian)
//2 Bind  Cinnect socket to mmy program start listenting for incomin comm
	if (bind (server_socket, (struct sockaddr *)&server_addr, 
	sizeof (server_addr)) < 0) {
		printf ("grrr, can't bind server socket\n");
		close (server_socket);
		return 2;
	}	/* endif */

	/*
	 * start listening on the socket
	 */
        //2.1 Listen (after Bind) whats bind?
	if (listen (server_socket, 5) < 0) {
		printf ("grrr, can't listen on socket\n");
		close (server_socket);
		return 3;
	}	/* endif */
//end of init
	/*
	 * for this server, run an endless loop that will
	 * accept incoming requests from a remote client.
	 * the server will fork a child copy of itself to handle the
	 * request, and the parent will continue to listen for the
	 * next request
	 */

	while (1) {
		
		/*
		 * accept a packet from the client
		 */
// Listening and found a client attempting to connect

//4. Accept
		client_len = sizeof (client_addr);
		if ((client_socket = accept (server_socket, 
		(struct sockaddr *)&client_addr, &client_len)) < 0) {
			printf ("grrr, can't accept a packet from client\n");
			close (server_socket);
			return 4;
		}	/* endif */

		

		/*
		 * fork a child
		 */

		if (fork() == 0) {
			/*
			 * this is done by CHILD ONLY!
			 *
			 * read a block of info max BUFSIZE. compare 
			 * against 3 commands: date, who, df
			 */

			read (client_socket, buffer, BUFSIZ);


			//char ack_mesg[] = "message received~!";
			//write(client_socket, ack_mesg, strlen(ack_mesg));
			
			/*
			 * process command, and obtain outgoing data
			 */

			//Chnage commands to what they actually do
			if (strcmp (buffer, "W") == 0) {
				strcpy (buffer, "forward\n");
				len = strlen (buffer);
				printf("Command:%s", buffer);
			} else if (strcmp (buffer, "A") == 0) {
				strcpy (buffer, "Left\n");
				len = strlen (buffer);
				printf("Command:%s", buffer);
			} else if (strcmp (buffer, "S") == 0) {
				strcpy (buffer, "Backwards\n");
				len = strlen (buffer);
				printf("Command:%s", buffer);
			} else if (strcmp (buffer, "D") == 0) {
				strcpy (buffer, "Right\n");
				len = strlen (buffer);
				printf("Command:%s", buffer);
			} else if (strcmp (buffer, "stream") == 0) {
				
				//Execute python script and read its output
				FILE *fp;
				char command[BUFSIZ];
				
				//modify command to include full path to main.py
				snprintf(command, sizeof(command), "python3 /home/zpi/tests/Projects-V/CV-Code/main.py");
				
				//open command for reading
				fp = popen(command, "r");
				if(fp == NULL) {
					printf("Failed to run command");
				} else {
					if(fgets(python_output, sizeof(python_output), fp) != NULL) {
						python_output[strcspn(python_output, "\n")] = 0;
						snprintf(buffer, BUFSIZ, "Direction from python: %s\n", python_output);
						printf("Direction recived from python code: %s\n", python_output);
					}
					pclose(fp);
				}
			} else {
				strcpy (buffer, "invalid command\n");
				len = strlen (buffer);
				
			}	/* endif */

			/*
			 * write data to client, close socket, and exit child app
			 */
			//write(client_socket, "Response  inbound", 18);
			write(client_socket, buffer, len);
			close(client_socket);
			return 0;
			
			
		} else {
			/*
			 * this is done by parent ONLY
			 */
			close(client_socket);
		}	/* endif */
	}	/* end while */
	return 0;
}	/* end main */

/*
int launch_stream() {
	if (chdir("/home/zpi/tests/Projects-V/CV-Code") !=0){
			perror("chdir failed");
			exit(1);
		}
		//execute the python script to stream camera
	execlp("python3", "python3", "main.py", (char *)NULL);
	perror("Error executing python script");
	exit(1);
	return 0;
}
*/






