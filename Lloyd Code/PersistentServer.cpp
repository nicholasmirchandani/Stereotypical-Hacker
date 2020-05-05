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
#include <chrono>

/*#include A threading library
#include Our socket connection library
#include Random*/

PersistentServer::PersistentServer(int LobbyNumberLimit, int MaxPlayersPerLobby, int PortNum) {
	
	this->LobbyNumberLimit = LobbyNumberLimit;
	this->MaxPlayersPerLobby = MaxPlayersPerLobby;
	this->PortNum = PortNum;

	ThreadContinue = true;

	activeLobbies = new vector<Lobby*>();
	lobbyCodesAndLobbies = new map<char*, Lobby*>();
	
	// Start ClientConnector;
	connector = new std::thread(&PersistentServer::ClientConnector, this);
	deadLobbyDetector = new std::thread(&PersistentServer::DeadLobbyDetector, this);
}

PersistentServer::~PersistentServer() {
	
	ThreadContinue = false;

	delete activeLobbies;
	delete lobbyCodesAndLobbies;

	connector->join();
	deadLobbyDetector->join();
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
			if (activeLobbies->size() == 0)
				std::cout << "No lobbies\n";
			for (int i = 0; i < activeLobbies->size(); ++i) {
				std::cout << activeLobbies->at(i)->RoomCode();
				if (activeLobbies->at(i)->InGame()) {
					std::cout << " - In game";
				} else {
					std::cout << " - In lobby";
				}
				std::cout << std::endl;

			}
		}

	}
	//Accept input from user admin to display number of lobbies, active connections, etc;
	//Not critical to server functionality, just maintenence and such;

}

void PersistentServer::DeadLobbyDetector() {

	while (ThreadContinue) {
		std::this_thread::sleep_for(std::chrono::seconds(2));
		for (int i = activeLobbies->size()-1; i > -1; --i) {
			Lobby* l = activeLobbies->at(i);
			if (!l->IsActive()) {
				char* roomCode = l->RoomCode();
				activeLobbies->erase(activeLobbies->begin() + i);
				lobbyCodesAndLobbies->erase(roomCode);
				delete l;
			}
		}
	}

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
		client->WriteToStream((void*)toClient, strlen(toClient));

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
			continue;
		} else {
			int i = 0;
			while (tok != NULL && i < 25) {
				args[i] = tok;
				tok = strtok(NULL, " ");
				++i;
			}
		}

		if (strcmp(args[0], "NEWLOBBY") == 0) {
			
			if (activeLobbies->size() < LobbyNumberLimit) {
				AddLobby(client);

			} else {
				strcpy(toClient, "ERR SERVERFULL");
				client->WriteToStream((void*)toClient, strlen(toClient));
				client->KillConnection();
				continue;
			}

		} else if (strcmp(args[0], "JOINLOBBY") == 0) {
			
			char* tok = strtok(args[1], "\n");

			bool lobbyfound = false;
			Lobby* lobbytojoin;
			for (std::map<char*, Lobby*>::iterator i = lobbyCodesAndLobbies->begin(); i != lobbyCodesAndLobbies->end(); ++i) {
				if (strcmp((*i).first, tok) == 0) {
					lobbyfound = true;
					lobbytojoin = (*i).second;
				}			
			}
			if (!lobbyfound) {
				strcpy(toClient, "ERR BADROOMCODE");
				client->WriteToStream((void*)toClient, strlen(toClient));
				client->KillConnection();
				continue;
			} else if (lobbytojoin->PlayerCount() == MaxPlayersPerLobby){
				strcpy(toClient, "ERR LOBBYFULL");
				client->WriteToStream((void*)toClient, strlen(toClient));
				client->KillConnection();
				continue;
			} else {
				strcpy(toClient, "OK");
				client->WriteToStream((void*)toClient, strlen(toClient));
				lobbytojoin->AddPlayer(client, false);
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

	char* roomCode = new char[5];
	roomCode[0] = rand() % 26 + 65;
	roomCode[1] = rand() % 26 + 65;
	roomCode[2] = rand() % 26 + 65;
	roomCode[3] = rand() % 26 + 65;
	roomCode[4] = '\0';
	while (lobbyCodesAndLobbies->count(roomCode) > 0) {
		roomCode[0] = rand() % 26 + 65;
		roomCode[1] = rand() % 26 + 65;
		roomCode[2] = rand() % 26 + 65;
		roomCode[3] = rand() % 26 + 65;
	}

	Lobby* newlobby = new Lobby(roomCode, MaxPlayersPerLobby, client, &ThreadContinue);

	(*lobbyCodesAndLobbies)[roomCode] = newlobby;

	char* toClient = new char[256];
	strcpy(toClient, "OK ");
	strcat(toClient, roomCode);
	client->WriteToStream((void*)toClient, strlen(toClient));
	activeLobbies->push_back(newlobby);

}


