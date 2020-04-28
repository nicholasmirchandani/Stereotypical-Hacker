/** Stereotypical-Hacker
 * Game.h
 * Lloyd Black
 * April 19, 2020
 * This class contains game functions that line up with commands that will be executed by players,
 *  keeps track of global scores, and other such things.
 */

#ifndef GAME
#define GAME

#include <iostream>
#include <vector>
#include "Player.h"
#include "VirtualServer.h"
#include "Directory.h"
#include "VirtualFile.h"

using namespace std;

class Game {

	private:
		vector<VirtualServer*>* servers; // Should be named 192.168.1.X, for all the root ones
		int serversRemaining;

	public:
		vector<Player*>* activePlayers; // update Lobby.cpp to integrate this whole thing with CD and dead connections
		Game(int virtualServerNumber, vector<Player*>* playerList); // Maybe read in a file to build the "servers"

		void LS(Player* p, char** args); // Lists items in current directory
		void READ(Player* p, char** args); // Prints contents of a file, accept cat as an alias
		void EXEC(Player* p, char** args); // johntheripper, pingsweep
		void CAP(Player* p, char** args); // Used for capturing a "server"
		void PWD(Player* p); // Prints working directory
		void CD(Player* p, char** args); // Update player's cwd
		void SSH(Player* p, char** args);
		void HELP(Player* p); // lists commands available for use


};

#endif