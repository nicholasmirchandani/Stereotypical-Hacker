// Stereotypical-Hacker
// Tile.cpp
// Definition for Tile class to use in a Tilemap.
// Logan Welsh
// 04/05/2020

#include "Tile.h"


Tile::Tile() {
	init(' ', 255, 0);
}

Tile::Tile(char newChr) {
	init(newChr, 255, 0);
}

Tile::Tile(char newChr, int newFg, int newBg) {
	init(newChr, newFg, newBg);
}

Tile::~Tile() {

}




void Tile::setChr(char newChr) {
	chr = newChr;
}

void Tile::setFg(int newFg) {
	fg = ((newFg >= 0) && (newFg < 256)) ? newFg : fg;
}

void Tile::setBg(int newBg) {
	bg = ((newBg >= 0) && (newBg < 256)) ? newBg : bg;
}




char Tile::getChr() {
	return chr;
}
int Tile::getFg() {
	return fg;
}
int Tile::getBg() {
	return bg;
}



/*
std::string Tile::getToPrint() {
	string fgStr = "\033[38;5;" + (string)fg + "m";
	string bgStr = "\033[48;5;" + (string)bg + "m";
	return fgStr + bgStr + (string)chr;
}
*/





void Tile::init(char newChr, int newFg, int newBg) {
	setChr(newChr);
	setFg(newFg);
	setBg(newBg);
}