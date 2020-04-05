// Stereotypical-Hacker
// TilemapTest.cpp
// Test for terminal Tilemap class.
// Logan Welsh
// 04/05/2020

// https://stackoverflow.com/questions/4184468/sleep-for-milliseconds

#include <iostream>
#include <unistd.h>
#include "Tilemap.h"

using namespace std;

int main(int argc, char** argv) {
	
	// SETUP
	Tilemap *tMap = new Tilemap(80, 24, ' ');	
	
	

	// DEMO
	tMap->updateTerminal();	// Show initial state of Tilemap.
	usleep(500000);
	
	tMap->drawBox(5,7,16,8,true,' ',2,3);	// Draw a box on the Tilemap...
	tMap->updateTerminal();	// ...and update the terminal.
	usleep(500000);
	
	tMap->drawBox(60,16,8,8,false,'?',4,5);	// Draw another box on the Tilemap...
	tMap->updateTerminal();	// ...and update the terminal.
	usleep(500000);
	
	tMap->drawBox(64,4,10,10,true,'!',1,7);	// Draw another box on the Tilemap...
	tMap->updateTerminal();	// ...and update the terminal.
	usleep(6000000);
	
	

	// CLEANUP
	delete tMap;	// Automatically clears and resets the screen.



	return 0;
}
