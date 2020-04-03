/** Stereotypical-Hacker
 * SocketConnection.h
 * Lloyd Black
 * April 2, 2020
 * A class to streamline the use of sockets/networking
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>

// Default, opens a server-side socket for listening
SocketConnection::SocketConnection() {

	// Open socket file descriptor using IPv4 and TCP
	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFD == -1) {
		fprintf(stderr, "Error: socket failed to open\n%s", strerror(errno));
		return;
	}

	// NOTE: SO_REUSEADDR and SO_REUSEPORT may become necessary if we run into issues with many clients connecting to a single port.

	mode = 's'; // For server

	// Binding socket to port 54367
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORTNUM);
	if (bind(socketFD, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		fprintf(stderr, "Error: socket binding failed\n%s", strerror(errno));
		return;
	}

	// Set socket up for listening
	if (listen(socketFD, 5) == -1) {
		fprintf(stderr, "Error: socket binding failed\n%s", strerror(errno));
		return;
	}


}

SocketConnection::SocketConnection(string ServerIP) {

	

}

SocketConnection::