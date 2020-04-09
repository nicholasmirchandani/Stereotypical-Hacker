/** Stereotypical-Hacker
 * PersistentServer.cpp
 * Lloyd Black
 * March 28, 2020
 * This class is responsible for running persistently on the server,
 *  listening for incoming connections, and managing the threads used to run lobbies.
 */

#include "PersistentServer.h"
#include "SocketConnection.h"

#include <stdio.h>
#include <string.h>

/*#include A threading library
#include Our socket connection library
#include Random*/

PersistentServer::PersistentServer(int LobbyNumberLimit, int MaxPlayersPerLobby) {
	this.LobbyNumberLimit = LobbyNumberLimit;
	this.MaxPlayersPerLobby = MaxPlayersPerLobby;
	ThreadContinue = true;

	//ActiveLobbies = new List;
	//LobbyCodesAndLobbies = new Dictionary;
	// Start ClientConnector;
}

PersistentServer::~PersistentServer() {
	ThreadContinue = false;
	//Wait until all lobby and listener threads have closed;
	//delete ActiveLobbies;
	//delete LobbyCodesAndLobbies;
}

void PersistentServer::ServerFunctions() {

	//Accept input from user admin to display number of lobbies, active connections, etc;
	//Not critical to server functionality, just maintenence and such;

}

//Need to wait until socket class is finished
void PersistentServer::ClientConnector() {

	SocketConnection* listeningSocket = new SocketConnection(PORTNUM);

	char toClient[256];
	char fromClient[256];

	while(ThreadContinue) {

		SocketConnection* client = listeningSocket->GetClientConnection();
		strcpy(toClient, "Connected to Server");
		client->WriteToStream((void*)msgToClient, strlen(toClient));

		strcpy(fromClient, (char*)client->ReadFromStream(256)); // Potential issue if the message received from client doesn't have a terminating null character
		// continue from here, lloyd

		if (args[0] == "NEWLOBBY") {
			if (ActiveLobbies.count < LobbyNumberLimit) {
				AddLobby(client);
			} else {
				Send "Server full" to client;
				kill client;
			}
		} else if (args[0] == "JOINLOBBY") {
			if (args[1] not in LobbyCodesAndLobbies) {
				send "No Lobby with [RequestedRoomCode] found" to client;
				kill client;
			} else if (LobbyCodesAndLobbies[args[1]].PlayerCount() == MaxPlayersPerLobby){
				send "Lobby Full" to client;
				kill client;
			} else {
				LobbyCodesAndLobbies[args[1]].AddPlayer(client, false);
			}
		}

	}

	listeningSocket->KillConnection();
	listeningSocket = NULL;
	delete listeningSocket;

}

void PersistentServer::AddLobby(client) {

	srand(time(NULL));
	char first = rand() % 26 + 65;
	char second = rand() % 26 + 65;
	char third = rand() % 26 + 65;
	char fourth = rand() % 26 + 65;

	string roomCode = first + second + third + fourth;
	LobbyCodesAndLobbies[RoomCode] = new Lobby(RoomCode, MaxPlayersPerLobby, Client);

	send "Lobby created with Room Code [RoomCode]" to client;
	add client to LobbyCodesAndLobbies;

}
