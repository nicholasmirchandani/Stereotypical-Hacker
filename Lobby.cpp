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

Lobby::Lobby(char* roomCode, int MaxPlayers, SocketConnection* HostClient) {

	this->roomCode = roomCode;
	this->MaxPlayers = MaxPlayers;
	playerCount = 0;

	PlayersInLobby = new std::vector<Player*>();
	AddPlayer(HostClient, true); // Keep host player at front to check for security of Lobby commands

	std::thread LobbyThread(LobbyLoop);

}

Lobby::~Lobby() {

	//Stop LobbyLoop
	lobbyStillAlive = false;

	// Delete clients in lobby
	for (int i = playerCount-1; i > -1 ; --i) {

		PlayersInLobby->at(i)->KillPlayer();
		PlayersInLobby->erase(i);

	}

	delete PlayersInLobby;

}

void Lobby::LobbyLoop() {

	while(ThreadContinue && PlayerCount > 0) {

		if (gameActive && game->serversRemaining == 0) {
			
			std::string temp = "";

			gameActive = false;
			vector<Player*>* winners;
			winners->push_back(PlayersInLobby->at(0));

			for (int i = 1; i < playerCount; ++i) {
				if (PlayersInLobby->at(i)->score > winners->at(0)->score) {
					winners->erase(winners->begin(), winners->end());
					winners->push_back(PlayersInLobby->at(i));
				} else if (PlayersInLobby->at(i)->score == winners->at(0)->score) {
					winners->push_back(PlayersInLobby->at(i));
				}
			}

			if (winners->size() > 1) {
				temp += "GAME OVER\n";
				temp += "It's a tie! With a score of ";
				temp += winners->front()->score;
				temp += ", the winners are ";
				for (int i = 0; i < winners->size(); ++i) {
					temp += winners->at(i)->DispayName();
					temp += ", ";
					if (i == winners->size()-2) {
						temp += "and ";
					}
				}
				temp += '\n';
			} else {
				temp += "GAME OVER\n";
				temp += "With a score of ";
				temp += winners->front()->score;
				temp += ", the winner is ";
				temp += winners->front->DisplayName();
				temp += '\n';
			}

			char* toPlayer = new char[temp.size()+1];
			copy(temp.begin(), temp.end(), toPlayer);
			toPlayer[temp.size()] = '\0';

			for (int i = 0; i < playerCount; ++i) {
				PlayersInLobby->at(i)->SendToPlayer(toPlayer);
			}

		delete game;

		}

		// Flexible space to be used however.

	}

}

void Lobby::RunGame() {

	// Kick off the game logic here.
	Player* p;
	for (vector<Player*>::iterator i = PlayersInLobby->begin(); i != PlayersInLobby->end(); ++i) {
		p = *i;
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

			if (strcmp(args[0], "ls") == 0) {
				game->LS(player, args); 
			} else if (strcmp(args[0], "read") == 0) {
				game->READ(player, args);
			} else if (strcmp(args[0], "exec") == 0) {
				game->EXEC(player, args);
			} else if (strcmp(args[0], "cap") == 0) {
				game->CAP(player, args);
			} else if (strcmp(args[0], "pwd") == 0) {
				game->PWD(player);
				free(args);
			} else if (strcmp(args[0], "cd") == 0) {
				game->CD(player, args);
			} else if (strcmp(args[0], "ssh") == 0) {
				game->SSH(player, args);
			} else if (strcmp(args[0], "help") == 0) {
				game->HELP(player);
				free(args);
			} else if (strcmp(args[0], "quit") == 0) {
				player->KillPlayer();
				free(args);
			}
			
		}

		else if (!gameActive) { // non-game commands
			
			if (strcmp(args[0], "rename") == 0) {
				player->changeName(args[1]);
				free(args);
			} else if (strcmp(args[0], "chat") == 0) {
				Player* p;
				for (vector<Player*>::iterator i = PlayersInLobby->begin(); i != PlayersInLobby->end(); ++i) {
					
					p = *i;

					std::string temp = "";
					temp += p->DisplayName();
					temp += " - ";
					temp += args[1]; // Needs some way to keep words within quotes as one char* (from ReadFromPlayer)
					
					char* toPlayer = new char[temp.size()+1];
					copy(temp.begin(), temp.end(), toPlayer);
					toPlayer[temp.size()] = '\0';

					p->SendToPlayer(toPlayer);

				}
				free(args);
			} else if (strcmp(args[0], "quit") == 0) {
				player->KillPlayer();
				free(args);
			} else if (strcmp(args[0], "help") == 0) {
				std::string temp = " --- LOBBY COMMANDS ---\n\n";
				temp += "rename - Change your display name\n";
				temp += "   Usage: rename <new-display-name>\n\n";
				temp += "chat - Send a message to all other players in the lobby\n";
				temp += "   Usage: chat '<message>'\n\n";
				temp += "quit - Exit the lobby and quit the game\n";
				temp += "   Usage: quit\n";
				if (player->IsHost()) {
					temp += "rungame - Starts game\n";
					temp += "   Usage: rungame\n\n";
				}

				char* toPlayer = new char[temp.size()+1];
				copy(temp.begin(), temp.end(), toPlayer);
				toPlayer[temp.size()] = '\0';

				player->SendToPlayer(toPlayer);

				free(args);
			} // more lobby commands

			else if (player->IsHost()) { // host commands

				if (strcmp(args[0], "rungame") == 0) {
					RunGame(playerCount + 4, PlayersInLobby);
					free(args);
				} else if (strcmp(args[0], "boot") == 0) {
					for (int i = 0; i < playerCount; ++i) {
						char* tok = strtok(args[1], "\n");
						if (strcmp(PlayersInLobby->at(i)->DisplayName(), tok)) {
							PlayersInLobby->at(i)->KillPlayer();
						}
					}
					free(args);
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

char* Lobby::RoomCode() {
	return roomCode;
}