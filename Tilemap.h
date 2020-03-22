// Stereotypical-Hacker
// Tilemap.h
// Header for terminal Tilemap class.
// Logan Welsh
// 03/21/2020


using namespace std;

class Tilemap {

	public:
		Tilemap();
		Tilemap(int newX, int newY);
		Tilemap(int newX, int newY, char initChar);
		~Tilemap();
		
		void setChar(int atX, int atY, char newChar);
		char getChar(int atX, int atY); 
		
		void updateTerminal();
		void fillMap(char newChar);
	
	
	private:
	
		int x;
		int y;
		
		char** map;
		
		void init(int newX, int newY, char initChar);

		void initializeMap(char initChar);

};