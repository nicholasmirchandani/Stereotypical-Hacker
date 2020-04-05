/** Stereotypical-Hacker
 * Lobby.h
 * Lloyd Black
 * March 28, 2020
 * This class encapsulates all lobby info, including room codes, lists of
 *  players, and logic for lobby operations and game running.
 */

#ifndef LOBBY
#define LOBBY

//#include Our socket connection library
#include "Player.h"

class Lobby {

	private:

		int MaxPlayers;
		string RoomCode;
		int playerCount;

		bool GameActive; // Used to handle clients joining mid-game

		std::list<Player> PlayersInLobby; // A list of clients in the lobby
		//ThreadID LobbyThread;

	public:
		Lobby(string RoomCode, int MaxPlayers, Player host);
		~Lobby();

		void LobbyLoop(); // Started in new thread in constructor.
		void RunGame();

		void AddPlayer(Socket client, bool isHost);
		void ClientListener(Socket client, bool isHost);


};

#endif
