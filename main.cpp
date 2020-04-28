#include "Directory.h"
#include "VirtualServer.h"
#include "Player.h"
#include "Game.h"
#include <iostream>
#include <vector>

using namespace std;

int main (int argc, char** argv) {

	Player* player = new Player(true, (char*)"itMe");
	bool gameActive = false, isHost = true;
	char** args;

	vector<Player*>* PlayersInLobby = new vector<Player*>();
	PlayersInLobby->push_back(player);

	Game* game;

	player->SendToPlayer((char*)"Welcome to the game\n");
	
	while (true) {

		
		args = player->ReadFromPlayer();

		if (gameActive) { // game commands

			if (strcmp(args[0], "ls") == 0 || strcmp(args[0], "ls\n") == 0) {
				game->LS(player, args); 
			} else if (strcmp(args[0], "read") == 0 || strcmp(args[0], "read\n") == 0) {
				game->READ(player, args);
			} else if (strcmp(args[0], "exec") == 0 || strcmp(args[0], "exec\n") == 0) {
				game->EXEC(player, args);
			} else if (strcmp(args[0], "cap") == 0 || strcmp(args[0], "cap\n") == 0) {
				game->CAP(player, args);
			} else if (strcmp(args[0], "pwd") == 0 || strcmp(args[0], "pwd\n") == 0) {
				game->PWD(player);
				free(args);
			} else if (strcmp(args[0], "cd") == 0 || strcmp(args[0], "cd\n") == 0) {
				game->CD(player, args);
			} else if (strcmp(args[0], "ssh") == 0 || strcmp(args[0], "ssh\n") == 0) {
				game->SSH(player, args);
			} else if (strcmp(args[0], "help") == 0 || strcmp(args[0], "help\n") == 0) {
				game->HELP(player);
				free(args);
			}
			
		}

		else if (!gameActive) { // non-game commands
			
			if (strcmp(args[0], "rename") == 0 || strcmp(args[0], "rename\n") == 0) {
				player->displayName = args[1];
				free(args);
			} else if (strcmp(args[0], "chat") == 0 || strcmp(args[0], "chat\n") == 0) {
				Player* p;
				for (vector<Player*>::iterator i = PlayersInLobby->begin(); i != PlayersInLobby->end(); ++i) {
					p = *i;
					string temp = "";
					temp += p->displayName;
					temp += " - ";
					temp += args[1]; // Needs some way to keep words within quotes as one char* (from ReadFromPlayer)
					
					char* toPlayer = new char[temp.size()+1];
					copy(temp.begin(), temp.end(), toPlayer);
					toPlayer[temp.size()] = '\0';

					p->SendToPlayer(toPlayer);

				}
				free(args);
			} else if (strcmp(args[0], "quit") == 0 || strcmp(args[0], "quit\n") == 0) {
				free(args);
				break;	
			} else if (strcmp(args[0], "help") == 0 || strcmp(args[0], "help\n") == 0) {
				string temp = "\n --- LOBBY COMMANDS ---\n\n";
				temp += "rename - Change your display name\n";
				temp += "   Usage: rename <new-display-name>\n\n";
				temp += "chat - Send a message to all other players in the lobby\n";
				temp += "   Usage: chat '<message>'\n\n";
				temp += "quit - Exit the lobby and quit the game\n";
				temp += "   Usage: quit\n";

				char* toPlayer = new char[temp.size()+1];
				copy(temp.begin(), temp.end(), toPlayer);
				toPlayer[temp.size()] = '\0';

				player->SendToPlayer(toPlayer);

				free(args);
			} // more lobby commands

			else if (player->IsHost()) { // host commands

				if (strcmp(args[0], "rungame") == 0 || strcmp(args[0], "rungame\n") == 0) {
					game = new Game(4, PlayersInLobby);
					gameActive = true;
					free(args);
				}

			}

		}

	}

}