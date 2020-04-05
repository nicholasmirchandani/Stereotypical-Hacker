/** Stereotypical-Hacker
 * Player.h
 * Lloyd Black
 * March 28, 2020
 * This class represents each player in a lobby, and encapsulates information such as
 *  lobby privilege and display name.
 */

class Player {

 private:
 	SocketConnection; // Represented in "Client" variable in other classes.
 	bool isHost;
 	string displayName;

 public:
 	Player(SocketConnection, bool isHost, string displayName);
 	~Player();

 	changeName(string newName);
 	SendToPlayer();
 	// etc

};

#endif
