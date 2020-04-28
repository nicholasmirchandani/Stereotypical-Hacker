/** Stereotypical-Hacker
 * Lobby.h
 * Lloyd Black
 * March 28, 2020
 * This class encapsulates all lobby info, including room codes, lists of
 *  players, and logic for lobby operations and game running.
 */

#ifndef LOBBY
#define LOBBY

#include "SocketConnection.h"
#include "Player.h"
#include "Game.h"

class Lobby {

	private:

		int MaxPlayers;
		char* roomCode;
		int playerCount;

		bool gameActive; // Used to handle clients joining mid-game
		Game* game;

		std::vector<Player*>* PlayersInLobby; // A list of clients in the lobby

		bool lobbyStillAlive = true;

	public:
		Lobby(char* RoomCode, int MaxPlayers, SocketConnection* host);
		~Lobby();

		void LobbyLoop(); // Started in new thread in constructor.
		void RunGame();

		void AddPlayer(SocketConnection* client, bool isHost);
		void ClientListener(SocketConnection* client, bool isHost);

		void KillLobby();

		bool IsActive();
		char* RoomCode();


};

#endif
