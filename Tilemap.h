// Stereotypical-Hacker
// Tilemap.h
// Header for terminal Tilemap class.
// Logan Welsh
// 03/21/2020

#include "Tile.h"

using namespace std;

class Tilemap {

	public:
		Tilemap();
		Tilemap(int newX, int newY);
		Tilemap(int newX, int newY, char initChar);
		~Tilemap();
		
		void setChar(int atX, int atY, char newChar);
		void setChar(int atX, int atY, char newChar, int fg, int bg);
		char getChar(int atX, int atY); 
		
		void updateTerminal();
		void fillMap(char newChar);
		void fillFg(int newFg);
		void fillBg(int newBg);
		
		void drawBox(int atX, int atY, int wi, int hi, bool hollow, char newChar, int fg, int bg);
	
	private:
	
		int x;
		int y;
		
		Tile*** map;
		
		void init(int newX, int newY, char initChar);

		void initializeMap(char initChar);

};