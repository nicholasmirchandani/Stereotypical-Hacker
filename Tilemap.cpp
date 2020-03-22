// Stereotypical-Hacker
// Tilemap.cpp
// Definition for terminal Tilemap class.
// Logan Welsh
// 03/21/2020

// https://stackoverflow.com/questions/936687/how-do-i-declare-a-2d-array-in-c-using-new
// https://www.csie.ntu.edu.tw/~r92094/c++/VT100.html


#include "Tilemap.h"
#include <iostream>

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
	initializeMap('?');
	cout << "\0337";	// savecursor DECSC; Save cursor position and attributes
}


Tilemap::~Tilemap() {
	delete map;
	cout << "\0338";	// restorecursor DECSC; Restore cursor position and attributes
}

void Tilemap::initializeMap(char initChar) {
	cout << "\033[2J\033[1;1H";	// Faking a clear screen using ANSI escape codes	
	map = new char*[y];
	for(int i = 0; i < y; ++i) {
		map[i] = new char[x];
		for(int j = 0; j < x; ++j) {
			map[i][j] = initChar;
		}
	}
}




void Tilemap::updateTerminal() {	
	cout << "\033[H";	// cursorhome; Move cursor to upper left corner
	for(int i = 0; i < y; ++i) {
		for(int j = 0; j < x; ++j) {
			cout << map[i][j];
		}
		if(i != y-1) {
			cout << "\n";	// nextline NEL; Move to next line
		}
	}
	
	cout << flush;

}

void Tilemap::fillMap(char newChar) {
	for(int i = 0; i < y; ++i) {
		for(int j = 0; j < x; ++j) {
			map[i][j] = newChar;
		}
	}
}

