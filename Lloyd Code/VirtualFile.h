/** Stereotypical-Hacker
 * VirtualFile.h
 * Lloyd Black
 * April 19, 2020
 * This class functions as a simulated file.
 */

#ifndef VIRTUAL_FILE
#define VIRTUAL_FILE

#include <iostream>

class VirtualFile {

	public:
		char filetype; // 't' for text, 'e' for executable
		char permissionsLevel; // 'a' for all

		char* name;
		char* contents;

		VirtualFile(char filetype, char permissionsLevel, char* name, char* contents) {
			this->filetype = filetype;
			this->permissionsLevel = permissionsLevel;
			this->name = name;
			this->contents = contents;
		}

};

#endif