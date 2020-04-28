// Player.h
// Used for testing the game functions and such

#include "Player.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

Player::Player(bool isHost, char* displayName)
{
  this->isHost = isHost;
  this->displayName = displayName;
  this->score = 0;
}

Player::~Player()
{
}

void Player::changeName(char* newName)
{
	displayName = newName;
}

void Player::SendToPlayer(char* toPlayer) {

	cout << toPlayer << endl;
	cout.flush();

}

// Converts input from a socket stream into a char*, denoting strings within single quotes as one argument
char** Player::ReadFromPlayer() {

	char** args = (char**)malloc(sizeof(char*) * 25); // MAKE SURE TO FREE ARGS WHEN DONE WITH THEM
	char* tok;
	
	string temp;
	getline(cin, temp);

	char* fromClient = new char[temp.size()+1];
	copy(temp.begin(), temp.end(), fromClient);
	fromClient[temp.size()] = '\0';

	tok = strtok(fromClient, " ");
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
			tok = strtok(NULL, " ");
		}
	}

	return args;

}

bool Player::IsHost() {
	return isHost;
}

char* Player::DisplayName() {

	return displayName;

}


