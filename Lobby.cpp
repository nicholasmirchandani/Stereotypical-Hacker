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

		if (gameActive && game->serversRemaining == 0) {
			gameActive = false;
			//Determine winners and send message to 
		}

		// Flexible space to be used however.

	}

}

void Lobby::RunGame() {

	// Kick off the game logic here.
	for (Player* p : PlayersInLobby) {
		p->SendToPlayer((char*)"GAME STARTING");
	}
	gameActive = true;
	game = new Game(MaxPlayers + 3, PlayersInLobby);
	// Maybe put a little bit of a break here

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
			} else if (strcmp(args[0], (char*)"read") || strcmp(args[0], (char*)"read\n")) {
				game->READ(player, args);
			} else if (strcmp(args[0], (char*)"exec") || strcmp(args[0], (char*)"exec\n")) {
				game->EXEC(player, args);
			} else if (strcmp(args[0], (char*)"cap") || strcmp(args[0], (char*)"cap\n")) {
				game->CAP(player, args);
			} else if (strcmp(args[0], (char*)"pwd") || strcmp(args[0], (char*)"pwd\n")) {
				game->PWD(player);
			} else if (strcmp(args[0], (char*)"cd") || strcmp(args[0], (char*)"cd\n")) {
				game->CD(player, args);
			} else if (strcmp(args[0], (char*)"ssh") || strcmp(args[0], (char*)"ssh\n")) {
				game->SSH(player, args);
			} else if (strcmp(args[0], (char*)"help") || strcmp(args[0], (char*)"help\n")) {
				game->HELP(player);
			}
			
		}

		else if (!gameActive) { // non-game commands
			
			if (strcmp(args[0], (char*)"rename") || strcmp(args[0], (char*)"rename\n")) {
				p->displayName = args[1];
			} else if (strcmp(args[0], (char*)"chat") || strcmp(args[0], (char*)"chat\n") {
				for (Player* p : PlayersInLobby) {
					string temp = "";
					temp += p->displayName;
					temp += " - ";
					temp += args[1]; // Needs some way to keep words within quotes as one char* (from ReadFromPlayer)
					
					char* toPlayer = new char[temp.size()+1];
					copy(temp.begin(), temp.end(), toPlayer);
					toPlayer[temp.size()] = '\0';

					p->SendToPlayer(toPlayer);

				}
			} // more in game commands

			else if (player->IsHost()) { // host commands

				if (strcmp(args[0], (char*)"rungame") || strcmp(args[0], (char*)"rungame\n")) {
					RunGame();
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