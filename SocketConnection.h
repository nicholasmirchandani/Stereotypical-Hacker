/** Stereotypical-Hacker
 * SocketConnection.h
 * Lloyd Black
 * April 2, 2020
 * A class to streamline the use of sockets/networking
 */

// NOTE FOR ALL NON-LLOYD PEOPLE: No need to implement these, I'm in the process of implementing all this
//   Let me know if anybody wants any other functionality, but I think this should cover most of what we want
#ifndef SOCKET_CONNECTION
#define SOCKET_CONNECTION

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <cstdlib>

#define PORTNUM 54367 // A random port number

template <class T>
class SocketConnection {
	
	private:
		int socketFD;
		char mode; // EG Server or Client
		bool connected;

	public:
		SocketConnection(); // Server constructor
		SocketConnection(int fd); // Constructor for individual connections from the server to clients
		SocketConnection(string ServerIP); // Client constructor

		void WriteToStream(T Data); // Writes data of type T to stream
		T ReadFromStream(); // Returns data read from stream
		SocketConnection<T> GetClientConnection(); // Used only in servers, returns an individual connection to a client

		string GetDestIP();
		string GetSourceIP();
		bool GetConnectionStatus();
		void KillConnection();

};

#endif