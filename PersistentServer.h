/** Stereotypical-Hacker
 * PersistentServer.h
 * Lloyd Black
 * March 28, 2020
 * This class is responsible for running persistently on the server,
 *  listening for incoming connections, and managing the threads used to run lobbies.
 */

#ifndef PERSISTENT_SERVER
#define PERSISTENT_SERVER

#define PORTNUM 54367 // can be changed later if we want to specify different port numbers every time

//#include A threading library
//#include Our socket connection library
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <map>
#include <thread>

#include "Lobby.h"
#include "SocketConnection.h"

class PersistentServer {

	private:

		int LobbyNumberLimit;
		int MaxPlayersPerLobby;
		int PortNum;

		std::vector<Lobby*>* activeLobbies;
		std::map<char*, Lobby*>* lobbyCodesAndLobbies; // Dictionary tracking lobbies by room code and references to the Lobby objects

		bool ThreadContinue; // All threads run until ThreadContinue == false, Maybe a reference to one or something.

		std::thread* connector;
		std::thread* serverLoop;

		//ThreadID LobbyListener;

	public:
		PersistentServer(int LobbyNumberLimit, int MaxPlayersPerLobby);
		~PersistentServer();

		void ServerLoop(); // A loop for managing threads and other server functions without being blocked on waiting for connections

		void ClientConnector(); // A separate thread from the server loop, waits for clients attempting to connect

		void AddLobby(SocketConnection* client); // Generates a new room code, client list, and lobby thread.

};

#endif
