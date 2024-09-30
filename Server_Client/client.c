/*
 * client.c
 *
 * This is a sample internet client application that will talk
 * to the server s.c via port reconfigurable port numner
 */

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

#define RPI_IP "192.168.1.226"
#define PORT 5000

char buffer[BUFSIZ];

int
main (void)
{

	int client_socket, len;
	struct sockaddr_in server_addr;


	// Socket init. routine

	/*
	 * get a socket for communications
	 */
	// 1. create (missing 2. Bind)
	if ((client_socket = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		printf ("grrr, can't get a client socket!\n");
		return 3;
	}	/* endif */

	/*
	 * initialize struct to get a socket to host
	 */
	
	// Filling in the form
	
	memset (&server_addr, 0, sizeof (server_addr));
	server_addr.sin_addr.s_addr = inet_addr(RPI_IP); 
	// "192.168.0.254" (string )=> 192.168.0.254
	server_addr.sin_family = AF_INET; // Choose TCP/Ip com mode
	server_addr.sin_port = htons(PORT); // "8080' => 8080 inot Big Endian

	/*
	 * attempt a connection to server
	 */

	//3. Connect
	if (connect (client_socket, (struct sockaddr *)&server_addr,
	sizeof (server_addr)) < 0) {
		printf ("grrr, can't connect to server!\n");
		close (client_socket);
		return 4;
	}	/* endif */
	
	// End of socket init. routine

	/*
	 * now that we have a connection, get a commandline from
	 * the user, and fire it off to the server
	 */

	

	printf ("Enter a command [date | who | df ]: ");
	fflush (stdout);
	fgets (buffer, sizeof (buffer), stdin);
	if (buffer[strlen (buffer) - 1] == '\n')
		buffer[strlen (buffer) - 1] = '\0';

	// Socket Send : wrap you buffer content into TCP parcel, send it out
	write (client_socket, buffer, strlen (buffer));

	// socket recieve: Unwarpping recieved parcel into applicatopn level contents
	//		   Write it into the buffer just like scanf()
	
	
	strcpy(buffer, "It is Working!");
	write(client_socket, buffer, strlen(buffer));
	
	len = read (client_socket, buffer, sizeof (buffer));
	printf ("Result of command:\n%s\n\n", buffer);

	/*
	 * cleanup
	 */

	close (client_socket);
	printf ("Client is finished\n");

	return 0;
}	/* end main */



