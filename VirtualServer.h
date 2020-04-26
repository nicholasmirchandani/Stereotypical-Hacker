#ifndef VIRTUAL_SERVER
#define VIRTUAL_SERVER

#include <iostream>
#include <vector>
#include "Directory.h"

using namespace std;

class VirtualServer : public Directory {

	public:
		char* rootuser;
		char* rootpass;

		char** otherusers;
		char** otherpasses;
		int otherusernum;

		bool captured;

		VirtualServer(char* name, char* rootuser, char* rootpass, char** otherusers, char** otherpasses, int otherusernum) : Directory(name) {
			this->rootuser = rootuser;
			this->rootpass = rootpass;
			this->otherusers = otherusers;
			this->otherpasses = otherpasses;
			this->otherusernum = otherusernum;
			this->parent = NULL;
			this->captured = false;
		}

};

#endif