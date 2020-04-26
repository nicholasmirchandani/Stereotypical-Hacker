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

using namespace std;

class Directory {

	public:
		char* name;

		Directory* parent;
		
		// Some way of determining if a directory is occupied and when to start typing battles

		Directory(char* name);

		vector<Directory*>* subdirectories;
		vector<VirtualFile*>* files;

		void AddSubdir(Directory* subdir) {
			subdir->parent = this;
			subdirectories->push_back(subdir);
		}
		void AddFile(VirtualFile* file) {
			files->push_back(file);
		}

};

#endif