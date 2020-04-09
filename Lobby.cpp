/** Stereotypical-Hacker
 * Lobby.h
 * Lloyd Black
 * March 28, 2020
 * This class encapsulates all lobby info, including room codes, lists of
 *  players, and logic for lobby operations and game running.
 */

#include "Lobby.h"

Lobby::Lobby(string RoomCode, int MaxPlayers, SocketConnection HostClient) {

	this.RoomCode = RoomCode;
	this.MaxPlayers = MaxPlayers;
	playerCount = 0;

	//PlayersInLobby = new ClientList, size of MaxPlayers;
	AddPlayer(HostClient, isHost = true); // Keep host player at front to check for security of Lobby commands

	//std::thread LobbyThread(LobbyLoop);

}

Lobby::~Lobby() {

	Stop LobbyLoop;

	Delete ClientsInLobby;

}

void Lobby::LobbyLoop() {

	while(ThreadContinue && PlayerCount > 0) {

		// Flexible space to be used however.

	}

}

void Lobby::RunGame() {

	// Kick off the game logic here.

}

void Lobby::AddPlayer(SocketConnection client, bool isHost) {

	Player p = new Player(client, isHost, "SomeGeneratedDisplayName");
	PlayersInLobby.push_back(p);
	//std::thread clientThread(ClientListener, client, isHost);
	playerCount++;

}

void Lobby::ClientListener(SocketConnection client, bool isHost) {

	while (clientThread && /*Client is connected*/) {
		string command = input from client;
		switch (commmand) {
			go through normal command palette;
			default:
				if (isHost) {
					switch (command) {
						go through host command palette;
					}
				}
		}
	}

	// After connection broken, remove Player from List of Players, playerCount--, kill Client connection.
	// If host quits, assign new player to be the host.

}
