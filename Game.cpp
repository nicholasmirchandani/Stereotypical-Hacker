/** Stereotypical-Hacker
 * Game.cpp
 * Lloyd Black
 * April 19, 2020
 * This class contains game functions that line up with commands that will be executed by players,
 *  keeps track of global scores, and other such things.
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "Game.h"
#include "Directory.h"
#include "VirtualServer.h"
#include "VirtualFile.h"
// #include "Player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

// Sets up virtual servers/file systems and assigns players to each
Game::Game(int virtualServerNumber, vector<Player*>* playerList) {

	// ifstream config;
	// config.open("GameConfig.txt");

	activePlayers = playerList;
	servers = new vector<VirtualServer*>();
	serversRemaining = virtualServerNumber;

	int hostnum = 1;

	for (int servs = 0; servs < virtualServerNumber; ++servs) {

		ifstream usernames;
		usernames.open("usernames.txt");
		ifstream passwords;
		passwords.open("passwords.txt");

		char rootuser[50];
		char rootpass[50];

		char* otherusers[5];
		char* otherpasses[5];

		int totalusers = 0;

		srand(time(NULL));
		int userlimit = (rand()%600)+200;
		for (int i = 0; i <= userlimit; ++i) {
			if (totalusers < 5 && (rand()%10) == 0 && i < userlimit) {
				usernames.getline(otherusers[totalusers], 50);
				passwords.getline(otherpasses[totalusers++], 50);
				continue;
			}
			usernames.getline(rootuser, 50);
			passwords.getline(rootpass, 50);
		}
		usernames.close();
		passwords.close();

		char hostIP[15];
		sprintf(hostIP, (char*)"192.168.1.%d", (hostnum++*4));
		VirtualServer* serv = new VirtualServer(hostIP, rootuser, rootpass, otherusers, otherpasses, totalusers);

		// usr directory with usernames and little readable notes
		Directory* usr = new Directory((char*)"usr");
		serv->AddSubdir(usr);

		Directory* rootuserdir = new Directory(rootuser);
		usr->AddSubdir(rootuserdir);

		VirtualFile* rootuserfile = new VirtualFile('t', 'a', (char*)"notetoself.txt", (char*)"Keep your password on lock\nWouldn't want anyone getting root access\n");
		rootuserdir->AddFile(rootuserfile);

		for (int i = 0; i < totalusers; ++i) {

			Directory* otheruserdir = new Directory(otherusers[i]);
			usr->AddSubdir(otheruserdir);

			VirtualFile* otheruserfile = new VirtualFile('t', 'a', (char*)"notetoself.txt", (char*)"Man, those security briefings are getting tedious\nIt's not like I have access to anything important\n");
			otheruserdir->AddFile(otheruserfile);

		}

		for (int i = 0; i < activePlayers->size(); ++i) {

			Directory* otheruserdir = new Directory(activePlayers->at(i)->displayName);
			usr->AddSubdir(otheruserdir);

			VirtualFile* otheruserfile = new VirtualFile('t', 'a', (char*)"notefromsysadmin.txt", (char*)"Filthy, no-good hacker\nBoot off server on sight\n");
			otheruserdir->AddFile(otheruserfile);

		}

		// bin directory with executables
		Directory* bin = new Directory((char*)"bin");
		serv->AddSubdir(bin);

		VirtualFile* johntheripper = new VirtualFile('e', 'a', (char*)"johntheripper", NULL);
		bin->AddFile(johntheripper);

		VirtualFile* pingsweep = new VirtualFile('e', 'a', (char*)"pingsweep", NULL);
		bin->AddFile(pingsweep);


		// etc directory with passwd file
		Directory* etc = new Directory((char*)"etc");
		serv->AddSubdir(serv);

		int rootplacement = rand()%(totalusers+1);
		hash<string> strhasher;
		string temp = "";
		for (int i = 0; i < totalusers; ++i) {
			if (i == rootplacement) {
				temp += rootuser;
				temp += ':';
				temp += strhasher(string(rootpass));
				temp += '\n';
			}
			temp += otherusers[i];
			temp += ':';
			temp += strhasher(string(otherpasses[i]));
			temp += '\n';

		}
		char* content = new char[temp.size()+1];
		copy(temp.begin(), temp.end(), content);
		content[temp.size()] = '\0';
		VirtualFile* passwd = new VirtualFile('t', 'a', (char*)"passwd.txt", content);
		etc->AddFile(passwd);

		servers->push_back(serv);

	}

	for (int n = 0; n < activePlayers->size(); ++n) {

		activePlayers->at(n)->cwd = servers->at(n);

	}

}

// Lists directories in cwd
void Game::LS(Player* p, char** args) {

	string temp = "";
	for (Directory* dir : p->cwd->subdirectories) {
		temp += dir->name;
		temp += " ";
	}

	for (VirtualFile* vir : p->cwd->files) {
		temp += vir->name;
		temp += " ";
	}
	temp += '\n';

	char* toPlayer = new char[temp.size()+1];
	copy(temp.begin(), temp.end(), toPlayer);
	toPlayer[temp.size()] = '\0';

	p->SendToPlayer(toPlayer);

}

// Prints contents of a named file
void Game::READ(Player* p, char** args) {

	string temp = "";

	bool fileFound = false;

	char* filename = strtok(args[1], "\n");

	for (VirtualFile* vir : p->cwd->files) {
		
		if (fileFound)
			break;

		if (strcmp(vir->name, filename)) {
			fileFound = true;

			if (vir->filetype == 'e') {
				temp += vir->name;
				temp += " of type Executable.\nNo text to read.\n";
			} else if (vir->filetype == 't') {
				temp += "READING ";
				temp += vir->name;
				temp += '\n';
				temp += vir->contents;
				temp += '\n';
			}

		}

	}

	for (Directory* dir : p->cwd->subdirectories) {
		
		if (fileFound)
			break;
		
		if (strcmp(dir->name , filename)) {
			temp += dir->name;
			temp += " of type Directory.\nNo text to read.\n";
			fileFound = true;
		}

	}

	if (!fileFound) {
		temp += "No file named ";
		temp += filename;
		temp += " was found.\n";
	}

	
	char* toPlayer = new char[temp.size()+1];
	copy(temp.begin(), temp.end(), toPlayer);
	toPlayer[temp.size()] = '\0';

	p->SendToPlayer(toPlayer);

}

// Executes an executable file found in <virtual-server>/bin
void Game::EXEC(Player* p, char** args) {

	char* executable = strtok(args[1], "\n");

	string temp = "";

	if (strcmp((char*)"johntheripper", executable)) {
		
		bool fileFound = false;
		char* filename = strtok(args[2], "\n");
		for (VirtualFile* vir : p->cwd->files) {
			
			if (fileFound)
				break;
			
			if (strcmp(filename, vir->name)) {
				
				fileFound = true;
				
				if (strcmp(filename, (char*)"passwd")) {
					
					temp += "DECRYPTED USERNAMES AND PASSWORDS\n";
					
					Directory* serverfinder = p->cwd;
					while (serverfinder->parent != NULL) {
						serverfinder = serverfinder->parent;
					}
					VirtualServer* serverroot = (VirtualServer*)serverfinder;
					int rootplacement = rand()%(serverroot->otherusernum+1);
					for (int i = 0; i < serverroot->otherusernum; ++i) {
						if (i == rootplacement) {
							temp += serverroot->rootuser;
							temp += ':';
							temp += serverroot->rootpass;
							temp += '\n';
						}
						temp += serverroot->otherusers[i];
						temp += ':';
						temp += serverroot->otherusers[i];
						temp += '\n';
					}

				} else {
					
					temp += "Format error: ";
					temp += filename;
					temp += " does not contain delineated usernames and encrypted passwords\n";
				
				}
			
			}

		}

	} else if (strcmp((char*)"pingsweep", executable)) {

		temp += "SCANNING NETWORK\n";
		for (VirtualServer* serv : servers) {

			temp += "Host up at IP Address ";
			temp += serv->name;
			temp += '\n';

		}

	} else {

		temp += "Command not found\n";

	}

	char* toPlayer = new char[temp.size()+1];
	copy(temp.begin(), temp.end(), toPlayer);
	toPlayer[temp.size()] = '\0';

	p->SendToPlayer(toPlayer);

}

// If credentials match server's root credentials, captures server
void Game::CAP(Player* p, char** args) {

	char* submitteduser = args[1];
	char* submitteduser = strtok(args[2], "\n");

	string temp = "";

	Directory* serverfinder = p->cwd;
	while (serverfinder->parent != NULL) {
		serverfinder = serverfinder->parent;
	}
	VirtualServer* serverroot = (VirtualServer*)serverfinder;

	if (serverroot->captured) {
		temp += "Failed to capture server: server already captured\n";
	} else if (strcmp(serverroot->rootuser, submitteduser) && strcmp(serverroot->rootpass, submittedpass)) {
		temp += "SERVER CAPTURE SUCCESSFUL\n";
		serverroot->captured = true;
		serversRemaining--;
		p->score++;
	} else {
		temp += "Failed to capture server: invalid credentials\n";
	}

	char* toPlayer = new char[temp.size()+1];
	copy(temp.begin(), temp.end(), toPlayer);
	toPlayer[temp.size()] = '\0';

	p->SendToPlayer(toPlayer);

}

// Prints player's working directory
void Game::PWD(Player* p, char** args) {

	string temp = "";

	Directory* serverfinder = p->cwd;
	while (serverfinder != NULL) {
		temp = string(serverfinder->name) + "/" + temp;
		serverfinder = serverfinder->parent;
	}
	temp += '\n';

	char* toPlayer = new char[temp.size()+1];
	copy(temp.begin(), temp.end(), toPlayer);
	toPlayer[temp.size()] = '\0';

	p->SendToPlayer(toPlayer);

}

// Changes player's working directory
void Game::CD(Player* p, char** args) {

	char* destdir = strtok(args[1], "\n");

	string temp = "";

	if (strcmp(destdir, (char*)"..") && p->cwd->parent != NULL) {

		p->cwd = p->cwd->parent;
		temp += "Changed directory to ";
		temp += p->cwd->name;
		temp += "\n";

	} else {

		bool fileFound = false;

		for (Directory* dir : p->cwd->subdirectories) {
		
			if (fileFound)
				break;
			
			if (strcmp(dir->name , destdir)) {
				p->cwd = dir;
				temp += "Changed directory to ";
				temp += dir->name;
				temp += '\n';
				fileFound = true;
			}

		}

		if (!fileFound) {
			temp += "No directory named ";
			temp += destdir;
			temp += " was found\n";
		}

	}

	char* toPlayer = new char[temp.size()+1];
	copy(temp.begin(), temp.end(), toPlayer);
	toPlayer[temp.size()] = '\0';

	p->SendToPlayer(toPlayer);

}

// Changes player's working direcotry to a new Virtual Server, if server is occupied, start competitive typing
void Game::SSH(Player* p, char** args) {

	char* destserv = strtok(args[1], "\n");

	string temp = "";

	bool serverFound = false;

	for (VirtualServer* vir : servers) {

		if (serverFound)
			break;

		if (strcmp(vir->name, destserv)) {

			serverFound = true;

			int playersInDestServer = 0;
			for (Player* hacker : activePlayers) {
				if (strcmp(hacker->cvs, vir->name)) {
					playersInDestServer++;
				}
			}

			if (playersInDestServer >= 2) {
				temp += "Could not SSH to ";
				temp += vir->name;
				temp += ": Server overloaded\n";
			} else if (playersInDestServer == 1) {

				// Kick off Competitive typing game here, determine who gets to stay and who gets booted.

			} else {

				p->cwd = vir;
				p->cvs = vir;
				temp += "SSH to ";
				temp += vir->name;
				temp += " successful\n";

			}

		}

	}

	if (!serverFound) {

		temp += "No host with IP ";
		temp += destserv;
		temp += " was found\n";

	}

	char* toPlayer = new char[temp.size()+1];
	copy(temp.begin(), temp.end(), toPlayer);
	toPlayer[temp.size()] = '\0';

	p->SendToPlayer(toPlayer);

}

void Game::HELP(Player* p) {

	string temp = "  --- USER COMMANDS ---\n\n";
	
	temp += "ls - Lists all files and subdirectories in current directory\n";
	temp += "   Usage: ls\n\n";

	temp += "read - Prints contents of a text file to console\n";
	temp += "   Usage: read <text-file>\n\n";

	temp += "exec - Executes any executable file in /bin directory\n";
	temp += "   Usage: exec <executable-file> <executable-arguments>\n\n";

	temp += "cap - Captures server, given root credentials\n";
	temp += "   Usage: cap <username> <password>\n\n";

	temp += "pwd - Prints user's current directory to console\n";
	temp += "   Usage: pwd\n\n";

	temp += "cd - Changes user's current directory\n";
	temp += "   Usage: cd <destination-directory>  OR  cd ..  to move up a directory\n\n";

	temp += "ssh - Connects to new host on network\n";
	temp += "   Usage: ssh <host-IP>\n";

	char* toPlayer = new char[temp.size()+1];
	copy(temp.begin(), temp.end(), toPlayer);
	toPlayer[temp.size()] = '\0';

	p->SendToPlayer(toPlayer);

}


// Client side: recognize valid commands, only send through valid ones.