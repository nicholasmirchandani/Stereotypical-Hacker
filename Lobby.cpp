/** Stereotypical-Hacker
 * Lobby.h
 * Lloyd Black
 * March 28, 2020
 * This class encapsulates all lobby info, including room codes, lists of
 *  players, and logic for lobby operations and game running.
 */

#include "Lobby.h"
#include "SocketConnection.h"
#include "Player.h"
#include "Game.h"
#include <cstdlib>
#include <vector>

Lobby::Lobby(char* RoomCode, int MaxPlayers, SocketConnection* HostClient) {

	this.->RoomCode = RoomCode;
	this->MaxPlayers = MaxPlayers;
	playerCount = 0;

	PlayersInLobby = new std::vector<Player*>();
	AddPlayer(HostClient, true); // Keep host player at front to check for security of Lobby commands

	//std::thread LobbyThread(LobbyLoop);

}

Lobby::~Lobby() {

	//Stop LobbyLoop
	lobbyStillAlive = false;

	// Delete clients in lobby
	for (int i = PlayersInLobby->size()-1; i > -1 ; --i) {

		PlayersInLobby->at(i)->KillPlayer();
		PlayersInLobby->erase(i);

	}

	PlayersInLobby = NULL;
	delete PlayersInLobby;

}

void Lobby::LobbyLoop() {

	while(ThreadContinue && PlayerCount > 0) {

		// Flexible space to be used however.

	}

}

void Lobby::RunGame() {

	// Kick off the game logic here.
	gameActive = true;
	game = new Game(MaxPlayers + 3, PlayersInLobby);

}

void Lobby::AddPlayer(SocketConnection* client, bool isHost) {

	Player* p = new Player(client, isHost, "SomeGeneratedDisplayName");
	PlayersInLobby->push_back(p);
	std::thread clientThread(ClientListener, p, isHost);
	playerCount++;
}

void Lobby::ClientListener(Player* player, bool isHost) {

	char** args;

	while (player->IsAlive()) {
		args = player->ReadFromPlayer();

		if (gameActive) { // game commands

			if (strcmp(args[0], (char*)"ls") || strcmp(args[0], (char*)"ls\n")) {
				game->LS(player, args); 
			} // And so on for read, exec, cap, pwd, cd, ssh, and help
			
		}

		else if (!gameActive) { // non-game commands
			switch (args[0]) {
				go through normal command palette;
				default:
					if (isHost) {
						switch (args[0]) {
							go through host command palette;
						}
					}
			}
		}
	}


	// After connection broken, remove Player from List of Players, playerCount--, kill Client connection.
	// If host quits, assign new player to be the host.
	PlayersInLobby->remove(player);
	playerCount--;
	if (playerCount == 0) {
		this->KillLobby;
	} else if (player->IsHost()) {
		PlayersInLobby->first()->SwapHost();
	}

	player->KillPlayer();
	player = NULL;
	delete player;
	
	return;

}

void Lobby::KillLobby() {

	// End lobby, kill all players
	lobbyStillAlive = false;

}

bool Lobby::IsActive() {

	return lobbyStillAlive;

}