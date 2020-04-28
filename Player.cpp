#include "Player.h"
#include "SocketConnection.h"
#include "reading_failure.h"

Player::Player(SocketConnection* client, bool isHost, char* displayName)
{
  this->isHost = isHost;
  this->displayName = displayName;
  socket = client;
  isAlive = true;
  this->score = 0;
}

Player::~Player()
{
	if (socket->IsAlive())
		socket->KillConnection();
}

void Player::changeName(char* newName)
{
	displayName = newName;
}

void Player::SendToPlayer(char* toPlayer) {

	socket->WriteToStream((void*)toPlayer, strlen(toPlayer));

}

// Converts input from a socket stream into a char*, denoting strings within single quotes as one argument
char** Player::ReadFromPlayer() {

	char* fromClient = new char[500];
	char** args = (char**)malloc(sizeof(char*) * 25);
	char* tok;

	try {
		
		strcpy(fromClient, socket->ReadFromStream());
		
		tok = strtok(fromClient, " \n");
		int i  = 0;
		while (tok != NULL && i < 25) {
			if (tok[0] == '\'') {
				char temptok[450];
				strcpy(temptok, &tok[1]);
				strcat(temptok, " ");
				tok = strtok(NULL, "'");
				if (tok != NULL)
					strcat(temptok, tok);
				tok = temptok;
			} else {
				args[i++] = tok;
				tok = strtok(NULL, " \n");
			}
		}

		return args;

	} catch (reading_failure e) {
		if (!socket->IsAlive()) {
			isAlive = false;
		}
		return NULL;
	}

}

void Player::KillPlayer() {
	isAlive = false;
}

void Player::SwapHost() { // If player is host, player is made not host and vice versa
	isHost = !isHost;
}

bool Player::IsHost() {
	return isHost;
}

bool Player::IsAlive() {
	return isAlive;
}

char* Player::DisplayName() {
	return displayName;
}