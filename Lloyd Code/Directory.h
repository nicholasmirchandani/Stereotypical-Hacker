/** Stereotypical-Hacker
 * Directory.h
 * Lloyd Black
 * April 19, 2020
 * This class functions as a simulated directory.
 */

#ifndef DIRECTORY
#define DIRECTORY

#include <iostream>
#include <vector>
#include "VirtualFile.h"
#include <string.h>

using namespace std;

class Directory {

	public:
		char* name;

		Directory* parent;

		vector<Directory*> subdirectories;
		vector<VirtualFile*> files;
		
		Directory(char* name) {
			char* buff = new char[strlen(name)];
			strcpy(buff, name);
			this->name = buff;
			this->parent = NULL;
		}

		void AddSubdir(Directory* subdir) {
			subdir->parent = this;
			subdirectories.push_back(subdir);
		}
		void AddFile(VirtualFile* file) {
			files.push_back(file);
		}

};

#endif