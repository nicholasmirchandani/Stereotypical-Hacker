/** Stereotypical-Hacker
 * PersistentServer.cpp
 * Lloyd Black
 * March 28, 2020
 * This class is responsible for running persistently on the server,
 *  listening for incoming connections, and managing the threads used to run lobbies.
 */

#include "PersistentServer.h"
#include "SocketConnection.h"
#include "Lobby.h"
#include "reading_failure.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

/*#include A threading library
#include Our socket connection library
#include Random*/

PersistentServer::PersistentServer(int LobbyNumberLimit, int MaxPlayersPerLobby, int PortNum) {
	
	this->LobbyNumberLimit = LobbyNumberLimit;
	this->MaxPlayersPerLobby = MaxPlayersPerLobby;
	this->PortNum = PortNum;

	ThreadContinue = true;

	activeLobbies = new vector<Lobby*>();
	lobbyCodesAndLobbies = new map<char*, Lobby*>()
	
	// Start ClientConnector;
	connector = new std::thread(ClientConnector);
}

PersistentServer::~PersistentServer() {
	
	ThreadContinue = false;

	delete activeLobbies;
	delete lobbyCodesAndLobbies;

	connector->join();
	serverLoop->join();
	//Wait until all lobby and listener threads have closed;
}

void PersistentServer::ServerLoop() {

	string temp;

	while(true) {

		getline(cin, temp);

		if (temp == "shutdown") {
			ThreadContinue = false;
			break;
		} else if (temp == "printlobbies") {
			for (int i = 0; i < activeLobbies->size(); ++i) {
				cout << activeLobbies->at(i)->RoomCode() << endl;
			}
		}

	}
	//Accept input from user admin to display number of lobbies, active connections, etc;
	//Not critical to server functionality, just maintenence and such;

}

//Need to wait until socket class is finished
void PersistentServer::ClientConnector() {

	SocketConnection* listeningSocket = new SocketConnection(PortNum);

	char toClient[256]; // char buffer for sending data to clients
	char fromClient[256]; // char buffer for reading data from clients
	char* args[25]; // char* buffer for splitting data on spaces
	char* tok;

	while(ThreadContinue) {

		SocketConnection* client = listeningSocket->GetClientConnection();
		strcpy(toClient, "Connected to Server");
		client->WriteToStream((void*)msgToClient, strlen(toClient));

		// Maybe delegate reading/splitting data to a function
		try {
			strcpy(fromClient, (char*)client->ReadFromStream(256)); // Potential issue if the message received from client doesn't have a terminating null character
		} catch (reading_failure e) {
			client->KillConnection();
			continue;
		}

		tok = strtok(fromClient, " ");
		if (tok == NULL) { // empty message
			client->KillConnection();
			continue
		} else {
			int i = 0;
			while (tok != NULL && i < 25) {
				args[i] = tok;
				tok = strtok(fromClient, " ");
				++i;
			}
		}

		if (args[0] == "NEWLOBBY") {
			
			if (ActiveLobbies.count < LobbyNumberLimit) {
				AddLobby(client);
			} else {
				strcpy(toClient, "ERR SERVERFULL");
				client->WriteToStream((void*)msgToClient, strlen(toClient));
				client->KillConnection();
				continue;
			}

		} else if (args[0] == "JOINLOBBY") {
			
			std::map<RoomCode, Lobby>::iterator iter = LobbyCodesAndLobbies.find(args[1]);

			if (iter == LobbyCodesAndLobbies.end()) {
				strcpy(toClient, "ERR BADROOMCODE");
				client->WriteToStream((void*)toClient, strlen(toClient));
				client->KillConnection();
				continue;
			} else if (LobbyCodesAndLobbies[args[1]].PlayerCount() == MaxPlayersPerLobby){
				strcpy(toClient, "ERR LOBBYFULL");
				client->WriteToStream((void*)toClient, strlen(toClient));
				client->KillConnection();
				continue;
			} else {
				strcpy(toClient, "OK");
				client->WriteToStream((void*)toClient, strlen(toClient));
				LobbyCodesAndLobbies[args[1]].AddPlayer(client, false);
			}

		} else { // Invalid message from client

			client->KillConnection();
			continue;

		}

	}

	listeningSocket->KillConnection();
	listeningSocket = NULL;
	delete listeningSocket;

}

void PersistentServer::AddLobby(SocketConnection* client) {

	srand(time(NULL));

	char* roomCode = new char[4];
	roomCode[0] = rand() % 26 + 65;
	roomCode[1] = rand() % 26 + 65;
	roomCode[2] = rand() % 26 + 65;
	roomCode[3] = rand() % 26 + 65;

	LobbyCodesAndLobbies[roomCode] = new Lobby(roomCode, MaxPlayersPerLobby, client);

	char* toClient = new char[256];
	strcpy(toClient, "OK ");
	strcat(toClient, roomCode);
	client->WriteToStream((void*)toClient, strlen(toClient));
	LobbyCodesAndLobbies[roomCode].AddPlayer(client, true);

}


