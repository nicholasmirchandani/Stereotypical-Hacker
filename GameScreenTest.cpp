// Stereotypical-Hacker
// GameScreen.cpp
// Test for GameScreen class for typing mini-game.
// Logan Welsh
// 04/12/2020

#include "GameScreen.h"
#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char** argv) {

	int phraseAmt = 3;
	string phrases[phraseAmt] = {
		"No, I think we're just getting started.",
		"Sir; Finishing this fight.",
		"Wake me when you need me."
	};
	
	GameScreen *gScrn = new GameScreen();
	gScrn->getTilemap()->writeString(0, 0, "DEBUG INFO");
	gScrn->getTilemap()->writeString(0, 1, "Last key pressed:");
	gScrn->getTilemap()->writeString(0, 2, "Phrase number:");
	gScrn->getTilemap()->writeString(0, 3, "Misses:");
	gScrn->getTilemap()->writeString(0, 4, "Press '[' to skip a phrase (Debug only)");

	char userInput = 0;
	for(int i = 0; i < phraseAmt; ++i) {
		gScrn->changePhrase(phrases[i]);
		gScrn->updateTerminal();

		while(!gScrn->phraseComplete()) {
			

			system("stty raw");
			userInput = getchar();
			system("stty cooked");
			
			if(userInput == '[') {
				break;
			}
			gScrn->charTyped(userInput);

			//gScrn->getTilemap()->setChar(7, 0, userInput);
			gScrn->getTilemap()->setChar(19, 1, userInput);
			gScrn->getTilemap()->writeString(19, 2, to_string(i));
			gScrn->getTilemap()->writeString(19, 3, to_string(gScrn->getMisses()));

			gScrn->updateTerminal();
		}
	}
	
	delete gScrn;
	
	return 0;
}