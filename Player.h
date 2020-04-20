/** Stereotypical-Hacker
 * Player.h
 * Lloyd Black
 * March 28, 2020
 * This class represents each player in a lobby, and encapsulates information such as
 *  lobby privilege and display name.
 */

#include "SocketConnection.h"
#include <stdio.h>
#include <string.h>
#include <cstdlib>

class Player {

 private:
 	SocketConnection* socket; // Represented in "Client" variable in other classes.
 	std::string displayName;

 	bool isHost;
 	bool isAlive;

 	Directory* cwd;
 	char privilege;

 public:
 	Player(SocketConnection* client, bool isHost, std::string displayName);
 	~Player();

 	void changeName(std::string newName);
 	void SendToPlayer(char* toPlayer);
 	char** ReadFromPlayer(); // returns an array of words sent from client

 	void KillPlayer();

 	void SwapHost();

 	bool IsHost();
 	bool IsAlive();

};

#endif
