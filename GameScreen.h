// Stereotypical-Hacker
// GameScreen.h
// Header for GameScreen class for typing mini-game.
// Logan Welsh
// 04/12/2020

#include "Tilemap.h"
#include <string>
#include <thread>

using namespace std;

class GameScreen {
	
	public:
		GameScreen();
		GameScreen(string newPhrase);
		~GameScreen();

		void changePhrase(string newPhrase);
		Tilemap* getTilemap();
		int getMisses();

		bool charTyped(char typed);
		void updateTerminal();
		bool phraseComplete();
		
		void run(int phraseAmt, string* phrases, bool* endEarly, bool debug);	// Returns true upon successful completion, false upon forfeit.
		void runThread(int phraseAmt, string* phrases, bool* endEarly, bool debug);

	private:
		string phrase;
		int index;
		int misses;
		Tilemap *tMap;
		int boxX;
		int boxY;
		int boxW;
		int boxH;
	
		void init(string newPhrase, int newX, int newY, int newW, int newH);
		
		void writePhrase();
		void updateCursor(bool active);
};