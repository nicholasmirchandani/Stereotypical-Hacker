// Stereotypical-Hacker
// Tile.h
// Header for Tile class to use in a Tilemap.
// Logan Welsh
// 04/05/2020


using namespace std;

class Tile {

	public:

		Tile();
		Tile(char newChr);
		Tile(char newChr, int newFg, int newBg);
		~Tile();
		
		void setChr(char newChr);
		void setFg(int newFg);
		void setBg(int newBg);
		
		char getChr();
		int getFg();
		int getBg();

		//string getToPrint();
	
	private:

		char chr;
		int fg;
		int bg;
		
		void init(char newChr, int newFg, int newBg);
		
};