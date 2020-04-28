/** Stereotypical-Hacker
 * Player.h
 * Lloyd Black
 * March 28, 2020
 * This class represents each player in a lobby, and encapsulates information such as
 *  lobby privilege and display name.
 */

#ifndef PLAYER
#define PLAYER

#include "SocketConnection.h"
#include "Directory.h"
#include "VirtualServer.h"
#include <stdio.h>
#include <string.h>
#include <cstdlib>

class Player {

 private:
 	SocketConnection* socket; // Represented in "Client" variable in other classes.
 	char* displayName;

 	bool isHost;
 	bool isAlive;

 public:
 	Directory* cwd; // current working directory
 	VirtualServer* cvs; // current virtual server
 	int score;

 	Player(SocketConnection* client, bool isHost, char* displayName);
 	~Player();

 	void changeName(char* newName);
 	void SendToPlayer(char* toPlayer);
 	char** ReadFromPlayer(); // returns an array of words sent from client

 	void KillPlayer();

 	void SwapHost();

 	bool IsHost();
 	bool IsAlive();

 	char* DisplayName();

};

#endif
