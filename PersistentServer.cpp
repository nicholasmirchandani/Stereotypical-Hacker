/** Stereotypical-Hacker
 * PersistentServer.cpp
 * Lloyd Black
 * March 28, 2020
 * This class is responsible for running persistently on the server,
 *  listening for incoming connections, and managing the threads used to run lobbies.
 */

#include "PersistentServer.h"
/*#include A threading library
#include Our socket connection library
#include Random*/

PersistentServer::PersistentServer(int LobbyNumberLimit, int MaxPlayersPerLobby) {
	this.LobbyNumberLimit = LobbyNumberLimit;
	this.MaxPlayersPerLobby = MaxPlayersPerLobby;
	ThreadContinue = true;
	//ActiveLobbies = new List;
	//LobbyCodesAndLobbies = new Dictionary;
	//Start ClientConnector;
}

PersistentServer::~PersistentServer() {
	ThreadContinue = false;
	//Wait until all lobby and listener threads have closed;
	//delete ActiveLobbies;
	//delete LobbyCodesAndLobbies;
}

void PersistentServer::ServerFunctions() {

	//Accept input from user admin to display number of lobbies, active connections, etc;
	//Not critical to server functionality, just maintenence and such;

}

//Need to wait until socket class is finished
void PersistentServer::ClientConnector() {

	//open Socket(newlobbyport) as listeningSocket;

	while(ThreadContinue) {

		client = listeningSocket.acceptConnection();
		send "Connected to Server" to client;

		stringarray args = split(input from client);

		if (args[0] == "NEWLOBBY") {
			if (ActiveLobbies.count < LobbyNumberLimit) {
				AddLobby(client);
			} else {
				Send "Server full" to client;
				kill client;
			}
		} else if (args[0] == "JOINLOBBY") {
			if (args[1] not in LobbyCodesAndLobbies) {
				send "No Lobby with [RequestedRoomCode] found" to client;
				kill client;
			} else if (LobbyCodesAndLobbies[args[1]].PlayerCount() == MaxPlayersPerLobby){
				send "Lobby Full" to client;
				kill client;
			} else {
				LobbyCodesAndLobbies[args[1]].AddPlayer(client, false);
			}
		}

	}

	close listeningSocket;

}

void PersistentServer::AddLobby(client) {

	srand(time(NULL));
	int firstDigit = rand() % 26 + 1;
	int secondDigit = rand() % 26 + 1;
	int thirdDigit = rand() % 26 + 1;
	int fourthDigit = rand() % 26 + 1;

	char first, second, third, fourth;

	switch (firstDigit) {
		case 1: first = 'A';
						break;

		case 2: first = 'B';
						break;

		case 3: first = 'C';
						break;

		case 4: first = 'D';
						break;

		case 5: first = 'E';
						break;

		case 6: first = 'F';
						break;

		case 7: first = 'G';
						break;

		case 8: first = 'H';
						break;

		case 9: first = 'I';
						break;

		case 10: first = 'J';
						 break;

		case 11: first = 'K';
						 break;

		case 12: first = 'L';
						 break;

		case 13: first = 'M';
						 break;

		case 14: first = 'N';
						 break;

		case 15: first = 'O';
						 break;

		case 16: first = 'P';
						 break;

		case 17: first = 'Q';
						 break;

		case 18: first = 'R';
						 break;

		case 19: first = 'S';
						 break;

		case 20: first = 'T';
						 break;

		case 21: first = 'U';
						 break;

		case 22: first = 'V';
						 break;

		case 23: first = 'W';
						 break;

		case 24: first = 'X';
						 break;

		case 25: first = 'Y';
						 break;

		case 26: first = 'Z';
							break;
	}

	switch (secondDigit) {
		case 1: second = 'A';
						break;

		case 2: second = 'B';
						break;

		case 3: second = 'C';
						break;

		case 4: second = 'D';
						break;

		case 5: second = 'E';
						break;

		case 6: second = 'F';
						break;

		case 7: second = 'G';
						break;

		case 8: second = 'H';
						break;

		case 9: second = 'I';
						break;

		case 10: second = 'J';
						 break;

		case 11: second = 'K';
						 break;

		case 12: second = 'L';
						 break;

		case 13: second = 'M';
						 break;

		case 14: second = 'N';
						 break;

		case 15: second = 'O';
						 break;

		case 16: second = 'P';
						 break;

		case 17: second = 'Q';
						 break;

		case 18: second = 'R';
						 break;

		case 19: second = 'S';
						 break;

		case 20: second = 'T';
						 break;

		case 21: second = 'U';
						 break;

		case 22: second = 'V';
						 break;

		case 23: second = 'W';
						 break;

		case 24: second = 'X';
						 break;

		case 25: second = 'Y';
						 break;

		case 26: second = 'Z';
							break;
	}

	switch (thirdDigit) {
		case 1: third = 'A';
						break;

		case 2: third = 'B';
						break;

		case 3: third = 'C';
						break;

		case 4: third = 'D';
						break;

		case 5: third = 'E';
						break;

		case 6: third = 'F';
						break;

		case 7: third = 'G';
						break;

		case 8: third = 'H';
						break;

		case 9: third = 'I';
						break;

		case 10: third = 'J';
						 break;

		case 11: third = 'K';
						 break;

		case 12: third = 'L';
						 break;

		case 13: third = 'M';
						 break;

		case 14: third = 'N';
						 break;

		case 15: third = 'O';
						 break;

		case 16: third = 'P';
						 break;

		case 17: third = 'Q';
						 break;

		case 18: third = 'R';
						 break;

		case 19: third = 'S';
						 break;

		case 20: third = 'T';
						 break;

		case 21: third = 'U';
						 break;

		case 22: third = 'V';
						 break;

		case 23: third = 'W';
						 break;

		case 24: third = 'X';
						 break;

		case 25: third = 'Y';
						 break;

		case 26: third = 'Z';
							break;
	}

	switch (fourthDigit) {
		case 1: fourth = 'A';
						break;

		case 2: fourth = 'B';
						break;

		case 3: fourth = 'C';
						break;

		case 4: fourth = 'D';
						break;

		case 5: fourth = 'E';
						break;

		case 6: fourth = 'F';
						break;

		case 7: fourth = 'G';
						break;

		case 8: fourth = 'H';
						break;

		case 9: fourth = 'I';
						break;

		case 10: fourth = 'J';
						 break;

		case 11: fourth = 'K';
						 break;

		case 12: fourth = 'L';
						 break;

		case 13: fourth = 'M';
						 break;

		case 14: fourth = 'N';
						 break;

		case 15: fourth = 'O';
						 break;

		case 16: fourth = 'P';
						 break;

		case 17: fourth = 'Q';
						 break;

		case 18: fourth = 'R';
						 break;

		case 19: fourth = 'S';
						 break;

		case 20: fourth = 'T';
						 break;

		case 21: fourth = 'U';
						 break;

		case 22: fourth = 'V';
						 break;

		case 23: fourth = 'W';
						 break;

		case 24: fourth = 'X';
						 break;

		case 25: fourth = 'Y';
						 break;

		case 26: fourth = 'Z';
							break;
	}
	string roomCode = first + second + third + fourth;
	LobbyCodesAndLobbies[RoomCode] = new Lobby(RoomCode, MaxPlayersPerLobby, Client);

	send "Lobby created with Room Code [RoomCode]" to client;
	add client to LobbyCodesAndLobbies;

}
