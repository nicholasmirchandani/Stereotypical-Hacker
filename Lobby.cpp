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
#include <thread>
#include <algorithm>

Lobby::Lobby(char* roomCode, int MaxPlayers, SocketConnection* HostClient, bool* ThreadContinue) {

	this->roomCode = roomCode;
	this->MaxPlayers = MaxPlayers;
	playerCount = 0;

	AddPlayer(HostClient, true); // Keep host player at front to check for security of Lobby commands

	this->ThreadContinue = ThreadContinue;

	LobbyThread = new std::thread(&Lobby::LobbyLoop, this);

}

Lobby::~Lobby() {

	//Stop LobbyLoop
	lobbyStillAlive = false;

	// Delete clients in lobby
	for (int i = playerCount-1; i > -1 ; --i) {

		PlayersInLobby.at(i)->KillPlayer();
		PlayersInLobby.pop_back();

	}

	LobbyThread->join();
	delete LobbyThread;

}

void Lobby::LobbyLoop() {

	while(*ThreadContinue && playerCount > 0) {

		if (gameActive && game->ServersRemaining() == 0) {
			
			std::string temp = "";

			gameActive = false;
			vector<Player*> winners;
			winners.push_back(PlayersInLobby.at(0));

			for (int i = 1; i < playerCount; ++i) {
				if (PlayersInLobby.at(i)->score > winners.at(0)->score) {
					winners.erase(winners.begin(), winners.end());
					winners.push_back(PlayersInLobby.at(i));
				} else if (PlayersInLobby.at(i)->score == winners.at(0)->score) {
					winners.push_back(PlayersInLobby.at(i));
				}
			}

			if (winners.size() > 1) {
				temp += "GAME OVER\n";
				temp += "It's a tie! With a score of ";
				temp += winners.front()->score;
				temp += ", the winners are ";
				for (int i = 0; i < winners.size(); ++i) {
					temp += winners.at(i)->DisplayName();
					temp += ", ";
					if (i == winners.size()-2) {
						temp += "and ";
					}
				}
				temp += '\n';
			} else {
				temp += "GAME OVER\n";
				temp += "With a score of ";
				temp += winners.front()->score;
				temp += ", the winner is ";
				temp += winners.front()->DisplayName();
				temp += '\n';
			}

			char* toPlayer = new char[temp.size()+1];
			copy(temp.begin(), temp.end(), toPlayer);
			toPlayer[temp.size()] = '\0';

			for (int i = 0; i < playerCount; ++i) {
				PlayersInLobby.at(i)->SendToPlayer(toPlayer);
			}

		delete game;

		}

		// Flexible space to be used however.

	}

}

void Lobby::RunGame(int virtservs) {

	// Kick off the game logic here.
	Player* p;
	for (vector<Player*>::iterator i = PlayersInLobby.begin(); i != PlayersInLobby.end(); ++i) {
		p = *i;
		p->SendToPlayer((char*)"GAME STARTING");
	}
	game = new Game(virtservs, &PlayersInLobby);
	gameActive = true;
	// Maybe put a little bit of a break here

}

void Lobby::AddPlayer(SocketConnection* client, bool isHost) {

	Player* p = new Player(client, isHost, (char*)"SomeGeneratedDisplayName");
	PlayersInLobby.push_back(p);
	std::thread clientThread(&Lobby::ClientListener, this, p, isHost);
	clientThread.detach(); // Potentially dangerous; revisit when you feel like making a safe program
	playerCount++;

}

void Lobby::ClientListener(Player* player, bool isHost) {

	while (*ThreadContinue && player->IsAlive()) {

		char** args;
		args = player->ReadFromPlayer();
		if  (args == NULL) {
			break;
		}

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
				delete args; // free(args);
			} else if (strcmp(args[0], "cd") == 0) {
				game->CD(player, args);
			} else if (strcmp(args[0], "ssh") == 0) {
				game->SSH(player, args);
			} else if (strcmp(args[0], "help") == 0) {
				game->HELP(player);
				delete args; // free(args);
			} else if (strcmp(args[0], "quit") == 0) {
				player->KillPlayer();
				delete args; // free(args);
			}
			
		}

		else if (!gameActive) { // non-game commands
			
			if (strcmp(args[0], "rename") == 0) {
				player->changeName(args[1]);
				delete args; // free(args);
			} else if (strcmp(args[0], "chat") == 0) {

				std::string temp = "";
				temp += player->DisplayName();
				temp += " - ";
				temp += args[1];

				Player* p;
				for (vector<Player*>::iterator i = PlayersInLobby.begin(); i != PlayersInLobby.end(); ++i) {
					
					p = *i;
				
					char* toPlayer = new char[temp.size()+1];
					copy(temp.begin(), temp.end(), toPlayer);
					toPlayer[temp.size()] = '\0';

					p->SendToPlayer(toPlayer);

				}
				delete args; // free(args);
			} else if (strcmp(args[0], "quit") == 0) {
				player->KillPlayer();
				delete args; // free(args);
			} else if (strcmp(args[0], "listplayers") == 0) {
				std::string temp = "\n";
				for (int i = 0; i < PlayersInLobby.size(); ++i) {
					temp += PlayersInLobby.at(i)->DisplayName();
					temp += '\n';
				}
				temp += '\n';
				
				char* toPlayer = new char[temp.size()+1];
				copy(temp.begin(), temp.end(), toPlayer);
				toPlayer[temp.size()] = '\0';

				player->SendToPlayer(toPlayer);

				delete args;
			} else if (strcmp(args[0], "help") == 0) {
				std::string temp = "\n --- LOBBY COMMANDS ---\n\n";
				temp += "rename - Change your display name\n";
				temp += "   Usage: rename <new-display-name>\n\n";
				temp += "chat - Send a message to all other players in the lobby\n";
				temp += "   Usage: chat '<message>'\n\n";
				temp += "listplayers - Lists name of all players in lobby\n";
				temp += "   Usage: listplayers\n\n";
				temp += "quit - Exit the lobby and quit the game\n";
				temp += "   Usage: quit\n\n";
				if (player->IsHost()) {
					temp += "rungame - Starts game\n";
					temp += "   Usage: rungame\n\n";
					temp += "boot - Boots player from lobby\n";
					temp += "   Usage: boot <player-name>\n\n";

				}

				char* toPlayer = new char[temp.size()+1];
				copy(temp.begin(), temp.end(), toPlayer);
				toPlayer[temp.size()] = '\0';

				player->SendToPlayer(toPlayer);

				delete args; // free(args);
			} // more lobby commands

			else if (player->IsHost()) { // host commands

				if (strcmp(args[0], "rungame") == 0) {
					RunGame(playerCount + 4);
					delete args; // free(args);
				} else if (strcmp(args[0], "boot") == 0) {
					for (int i = 0; i < playerCount; ++i) {
						char* tok = strtok(args[1], "\n");
						if (strcmp(PlayersInLobby.at(i)->DisplayName(), tok) == 0) {
							PlayersInLobby.at(i)->KillPlayer();
						}
					}
					delete args; // free(args);
				}

			}

		}
	}


	// After connection broken, remove Player from List of Players, playerCount--, kill Client connection.
	// If host quits, assign new player to be the host.
	std::remove(PlayersInLobby.begin(), PlayersInLobby.end(), player);
	playerCount--;
	if (playerCount == 0) {
		this->KillLobby();
	} else if (player->IsHost()) {
		PlayersInLobby.front()->SwapHost();
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

bool Lobby::InGame() {
	return gameActive;
}

char* Lobby::RoomCode() {
	return roomCode;
}

int Lobby::PlayerCount() {
	return playerCount;
}