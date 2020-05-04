/** Stereotypical-Hacker
 * SocketConnection.h
 * Lloyd Black
 * April 2, 2020
 * A class to streamline the use of sockets/networking
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <cstdlib>

#include "SocketConnection.h"
#include "sending_failure.h"
#include "reading_failure.h"

/* Server Constructor: sets up a socket that listens for incoming connections
 * @param: int pn - the port number to listen for connections on
 */
SocketConnection::SocketConnection(int pn) {

	if (pn < 49152 || pn > 65535) {
		fprintf(stderr, "Error: port number not in valid range (49152-65535)\n");
	}
	portnum = pn;

	// Open socket file descriptor using IPv4 and TCP
	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFD == -1) {
		fprintf(stderr, "Error: socket failed to open\n%s", strerror(errno));
		return;
	}

	int temp = 1;
	int res = setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(int));
	if (res == -1) {
		fprintf(stderr, "Error: socket option (SO_REUSEADDR) failed\n%s", strerror(errno));
		return;
	}

	// NOTE: SO_REUSEADDR and SO_REUSEPORT w/ setsockopt() may become necessary if we run into issues with many clients connecting to a single port.
	mode = 'l'; // For "listener"

	// Binding socket to port 54367
	localAddressInfo.sin_family = AF_INET;
	localAddressInfo.sin_addr.s_addr = INADDR_ANY;
	localAddressInfo.sin_port = htons(portnum);
	socklen_t localAddressSize = sizeof(localAddressInfo);
	if (bind(socketFD, (struct sockaddr*)&localAddressInfo, localAddressSize) == -1) {
		fprintf(stderr, "Error: socket binding failed\n%s", strerror(errno));
		return;
	}

	// Set socket up for listening
	if (listen(socketFD, 5) == -1) {
		fprintf(stderr, "Error: socket listening failed\n%s", strerror(errno));
		return;
	}

	isAlive = true;

}

/* Constructor for individual connections from the server to clients
 * @param: int fd - a file descriptor generated by an 'l' mode SocketConnection from the "accept()" method.
 * @param: struct sockaddr_in localAddr - info about the local address of the socket (provided by listener)
 * @param: struct sockaddr_in peerAddr - info about the socket's peer's (provided by listener)
 */
SocketConnection::SocketConnection(int fd, struct sockaddr_in localAddr, struct sockaddr_in peerAddr) {

	socketFD = fd;
	localAddressInfo = localAddr;
	peerAddressInfo = peerAddr;
	mode = 's'; // For "server-side"
	isAlive = true;

}

/* Client-side Socket Constructor
 * @param: string serverIP - a string of the IP address to connect to
  * @param: int pn - the port number to connect to
 */
SocketConnection::SocketConnection(char* serverIP, int pn) {

	if (pn < 49152 || pn > 65535) {
		fprintf(stderr, "Error: port number not in valid range (49152-65535)\n");
	}
	portnum = pn;

	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFD == -1) {
		fprintf(stderr, "Error: socket failed to open\n%s", strerror(errno));
		return;
	}
	mode = 'c'; // For "client"

	// If we use REUSEADDR or REUSEPORT above, it's gotta be used down here too

	peerAddressInfo.sin_family = AF_INET;
	int ipConvRes = inet_pton(AF_INET, serverIP, (void*)&peerAddressInfo.sin_addr);
	if (ipConvRes == -1) {
		fprintf(stderr, "Error: converting IP to binary failed\n%s", strerror(errno));
		return;
	} else if (ipConvRes == 0) {
		fprintf(stderr, "Error: invalid IPv4 address\n");
		return;
	}
	peerAddressInfo.sin_port = htons(portnum);

	socklen_t peerAddressSize = sizeof(peerAddressInfo);
	if (connect(socketFD, (struct sockaddr*)&peerAddressInfo, peerAddressSize) == -1) {
		fprintf(stderr, "Error: connection to server failed\n%s", strerror(errno));
		return;
	}

	isAlive = true;

}

SocketConnection::~SocketConnection() {
	shutdown(socketFD, SHUT_RDWR);
	close(socketFD);
}


/* Writes data to socket
 * @param: void* data - pointer to data to send
 * @param: int len - the number of bytes of data to send
 */
void SocketConnection::WriteToStream(void* Data, int len) { // Note to self: may be some weirdness with pointers.

	if (mode == 'l') {
		return;
	}

	if (send(socketFD, Data, len, 0) == -1) {
		fprintf(stderr, "Error: message failed to send\n%s\n", strerror(errno));
		throw sending_failure("Data failed to send");	
	}

	return;

}

/* Reads data of type T from socket.
 * @param: int len - the number of bytes of data to read
 * @return: void* - Pointer to the data read from the socket
 */
char* SocketConnection::ReadFromStream(int len) {

	if (mode == 'l') {
		return NULL;
	}

	char* dest = new char[len];
	int res = read(socketFD, (void*)dest, len);
	if (res == -1) {
		fprintf(stderr, "Error: message failed to send\n%s", strerror(errno));
		throw reading_failure("Data failed to be read");
	} else if (res == 0) {
		fprintf(stderr, "Error when reading: Client connection closed\n");
		isAlive = false;
		throw reading_failure("Client connection closed");
	}

	return dest;

}


/* Used in listening SocketConnections, generates new socket connections used for actual communication
 * @return: SocketConnection* - a pointer to a new SocketConnection that can be used for communication between hosts.
 */
SocketConnection* SocketConnection::GetClientConnection() {

	if (mode != 'l') {
		return NULL;
	}

	socklen_t peerAddressSize = sizeof(peerAddressInfo);
	int newFD = accept(socketFD, (struct sockaddr*)&peerAddressInfo, &peerAddressSize);
	if (newFD == -1) {
		fprintf(stderr, "Error: connection to client failed\n%s", strerror(errno));
		return NULL;
	}

	SocketConnection* newSockConn = new SocketConnection(newFD, localAddressInfo, peerAddressInfo);
	return newSockConn;

}

void SocketConnection::KillConnection() {
	shutdown(socketFD, SHUT_RDWR);
	isAlive = false;
	close(socketFD);
}

bool SocketConnection::IsAlive() {
	return isAlive;
}
