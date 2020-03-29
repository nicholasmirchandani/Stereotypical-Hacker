/** Stereotypical-Hacker
 * Lobby.h
 * Lloyd Black
 * March 28, 2020
 * This class encapsulates all lobby info, including room codes, lists of
 *  players, and logic for lobby operations and game running.
 */

#ifndef LOBBY
#define LOBBY

#include Our socket connection library

class Lobby {

	private:
		
		int MaxPlayers;
		string RoomCode;
		int playerCount;

		bool GameActive; // Used to handle clients joining mid-game

		PlayersInLobby; // A list of clients in the lobby
		ThreadID LobbyThread;

	public:
		Lobby(RoomCode, MaxPlayers, /*"*/Host/*"*/Player);
		~Lobby();

		void LobbyLoop(); // Started in new thread in constructor.
		void RunGame();

		void AddPlayer(Client, bool isHost);
		void ClientListener(Client, bool isHost);


};

#endif