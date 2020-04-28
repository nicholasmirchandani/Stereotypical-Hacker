// TESTING CLASS ONLY

#ifndef PLAYER
#define PLAYER

#include "Directory.h"
#include "VirtualServer.h"
#include <stdio.h>
#include <string.h>
#include <cstdlib>

class Player {

 private:
 	bool isHost;
 	bool isAlive;

 public:
 	char* displayName;

 	Directory* cwd; // current working directory
 	VirtualServer* cvs; // current virtual server
 	int score;

 	Player(bool isHost, char* displayName);
 	~Player();

 	void changeName(char* newName);
 	void SendToPlayer(char* toPlayer);
 	char** ReadFromPlayer(); // returns an array of words sent from client
 	char* DisplayName();

 	bool IsHost();

};

#endif