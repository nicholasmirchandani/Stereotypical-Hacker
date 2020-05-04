// Stereotypical-Hacker
// GameScreen.cpp
// Definition for GameScreen class for typing mini-game.
// Logan Welsh
// 04/12/2020

#include "GameScreen.h"

GameScreen::GameScreen() {
	init(" ", 3, 12, 72, 10);
	
	// Placeholder box for opponent's progress:
	//tMap->drawBox(42, 12, 36, 10, true, ' ', 0, 1);
}

GameScreen::GameScreen(string newPhrase) {
	init(newPhrase, 4, 10, 20, 10);
}

GameScreen::~GameScreen() {
	delete tMap;
}

void GameScreen::init(string newPhrase, int newX, int newY, int newW, int newH) {
	boxX = newX;
	boxY = newY;
	boxW = newW;
	boxH = newH;
	tMap = new Tilemap(80, 24, ' ');
	tMap->drawBox(boxX, boxY, boxW, boxH, true, ' ', 0, 2);
	changePhrase(newPhrase);
	misses = 0;
}




void GameScreen::changePhrase(string newPhrase) {
	updateCursor(false);
	index = 0;
	phrase = newPhrase;
	writePhrase();
	updateCursor(true);
}

Tilemap* GameScreen::getTilemap() {
	return tMap;
}

int GameScreen::getMisses() {
	return misses;
}

bool GameScreen::charTyped(char typed) {
	if((int)typed == 127) {	// Ignore backspaces.
		return true;
	}
	if(typed == phrase[index]) {
		updateCursor(false);
		index++;
		updateCursor(true);
		return true;
	}
	else {
		misses++;
		return false;
	}
}



void GameScreen::updateTerminal() {
	updateCursor(true);
	tMap->updateTerminal();
	
	
}

bool GameScreen::phraseComplete() {
	return (index == phrase.size());
}



void GameScreen::writePhrase() {
	int charX = boxX + 1;
	int charY = boxY + 1;
	tMap->drawBox(charX, charY, boxW-2, boxH-2, false, ' ', 255, 0);
	for(int i = 0; i < phrase.size(); ++i) {
		tMap->setChar(charX + i%(boxW-2), charY + i/(boxW-2), phrase[i]);
	}
}

void GameScreen::updateCursor(bool active) {
	
	int charX = boxX + 1 + index%(boxW-2);
	int charY = boxY + 1 + index/(boxW-2);
	char c = tMap->getChar(charX, charY);
	int bg = (active) ? 5 : 0;
	tMap->setChar(charX, charY, c, 255, bg);
	
}


bool GameScreen::run(int phraseAmt, string* phrases, bool debug) {
	
	// Prepare info labels.
	this->getTilemap()->writeString(0, 1, "Last key pressed:");
	this->getTilemap()->writeString(0, 2, "Phrase number:");
	this->getTilemap()->writeString(0, 3, "Misses:");
	this->getTilemap()->writeString(0, 4, "Press ']' to forfeit the battle.");
	if(debug) {
		this->getTilemap()->writeString(0, 4, "Press '[' to skip a phrase (Debug only)");
	}

	// Loop for each phrase.
	char userInput = 127;	// Treat first input as a backspace.
	for(int i = 0; i < phraseAmt; ++i) {
		
		this->changePhrase(phrases[i]);
		this->updateTerminal();
		
		// While the phrase is not yet complete...
		while(!this->phraseComplete()) {
			
			//cout << (int)userInput;

			// ...get keypress from user...
			system("stty raw");
			userInput = getchar();
			system("stty cooked");
			
			// ...handle special keys...
			if(userInput == '[' && debug) {	// Skip a phrase in debug mode.
				break;
			}
			if(userInput == ']') {	// Forfeit this entire battle.
				return false;
			}
			
			
			this->charTyped(userInput);

			// ...and then update the screen.
			this->getTilemap()->setChar(19, 1, userInput);	// "Last key pressed:"
			this->getTilemap()->writeString(19, 2, to_string(i + 1) + " of " + to_string(phraseAmt));	// "Phrase number:"
			this->getTilemap()->writeString(19, 3, to_string(this->getMisses()));	// "Misses:"
			
			this->updateTerminal();
			
		}
	}
	
	return true;
}