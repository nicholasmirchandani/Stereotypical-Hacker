// Stereotypical-Hacker
// Tilemap.cpp
// Definition for terminal Tilemap class.
// Logan Welsh
// 03/21/2020

// https://stackoverflow.com/questions/936687/how-do-i-declare-a-2d-array-in-c-using-new
// https://www.csie.ntu.edu.tw/~r92094/c++/VT100.html


#include "Tilemap.h"



Tilemap::Tilemap() {
	init(80, 24, '?');
}

Tilemap::Tilemap(int newX, int newY) {
	init(newX, newY, '?');
}

Tilemap::Tilemap(int newX, int newY, char initChar) {
	init(newX, newY, initChar);
}

void Tilemap::init(int newX, int newY, char initChar) {
	x = (newX > 0) ? newX : 80;	// Only allows x-values greater than zero
	y = (newY > 0) ? newY : 24;	// Only allows y-values greater than zero
	initializeMap(initChar);
	cout << "\0337";	// savecursor DECSC; Save cursor position and attributes
}

Tilemap::~Tilemap() {
	delete map;
	cout << "\033[2J";	// clearscreen ED2; Clear entire screen
	cout << "\0338";	// restorecursor DECSC; Restore cursor position and attributes
}




void Tilemap::setChar(int atX, int atY, char newChar) {
	setChar(atX, atY, newChar, 255, 0);
}

void Tilemap::setChar(int atX, int atY, char newChar, int fg, int bg) {
	map[atY][atX]->setChr(newChar);
	map[atY][atX]->setFg(fg);
	map[atY][atX]->setBg(bg);
	
}

char Tilemap::getChar(int atX, int atY) {
	return map[atY][atX]->getChr();
}


void Tilemap::initializeMap(char initChar) {
	cout << "\033[2J\033[1;1H";	// Faking a clear screen using ANSI escape codes	
	map = new Tile**[y];
	for(int i = 0; i < y; ++i) {
		map[i] = new Tile*[x];
		for(int j = 0; j < x; ++j) {
			map[i][j] = new Tile(initChar);
		}
	}
}




void Tilemap::updateTerminal() {
	cout << "\033[H";	// cursorhome; Move cursor to upper left corner
	for(int i = 0; i < y; ++i) {
		for(int j = 0; j < x; ++j) {
			cout << "\033[38;5;" << map[i][j]->getFg() << "m";
			cout << "\033[48;5;" << map[i][j]->getBg() << "m";
			cout << map[i][j]->getChr();
		}
		if(i != y-1) {
			cout << "\n";
		}
		cout << flush;
	}
	
	//cout << flush;

	cout << "\033[38;5;255m";	// Reset foreground color (white)
	cout << "\033[48;5;0m";	// Reset background color (black)
}

void Tilemap::fillMap(char newChar) {
	for(int i = 0; i < y; ++i) {
		for(int j = 0; j < x; ++j) {
			map[i][j]->setChr(newChar);
		}
	}
}

void Tilemap::fillFg(int newFg) {
	for(int i = 0; i < y; ++i) {
		for(int j = 0; j < x; ++j) {
			map[i][j]->setFg(newFg);
		}
	}
}

void Tilemap::fillBg(int newBg) {
	for(int i = 0; i < y; ++i) {
		for(int j = 0; j < x; ++j) {
			map[i][j]->setBg(newBg);
		}
	}
}

void Tilemap::writeString(int atX, int atY, string str) {
	for(int i = 0; i < str.size(); ++i) {
		if(atX + i <= x) {
			setChar(atX + i, atY, str[i]);
		}
	}
}


void Tilemap::drawBox(int atX, int atY, int wi, int hi, bool hollow, char newChr, int newFg, int newBg) {
	for(int i = atY; i < (hi + atY); ++i) {
		for(int j = atX; j < (wi + atX); ++j) {
			bool interiorX = (j > atX) && (j < wi + atX - 1);
			bool interiorY = (i > atY) && (i < hi + atY - 1);
			if( !(hollow && interiorX && interiorY) ) {
				map[i][j]->setChr(newChr);
				map[i][j]->setFg(newFg);
				map[i][j]->setBg(newBg);
			}
		}
	}
}
