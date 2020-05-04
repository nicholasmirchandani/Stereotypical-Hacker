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

	gScrn->run(phraseAmt, phrases, false);
	
	delete gScrn;
	
	return 0;
}