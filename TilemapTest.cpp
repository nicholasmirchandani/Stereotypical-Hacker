// Stereotypical-Hacker
// TilemapTest.cpp
// Test for terminal Tilemap class.
// Logan Welsh
// 03/21/2020

// https://stackoverflow.com/questions/4184468/sleep-for-milliseconds

#include <iostream>
#include <unistd.h>
#include "Tilemap.h"

using namespace std;

int main(int argc, char** argv) {
	
	//cout << "\033[?7h";
	
	Tilemap *tMap = new Tilemap();
	
	char c = 'a';
	int fps = 3;
	
	unsigned int wait = static_cast<unsigned int>( (1 / static_cast<double>(fps)) * 1000000 );
	
	while(c - 1 != 'z') {
		
		tMap->fillMap(c);
		tMap->updateTerminal();
		
		usleep(wait);
		c++;
		
	}
	
	
	delete tMap;
	return 0;
}
