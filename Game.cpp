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
#include "Player.h"
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
	srand(time(NULL));

	int hostnum = 1;


	for (int servs = 0; servs < virtualServerNumber; ++servs) {

		ifstream usernames("usernames.txt");
		ifstream passwords("passwords.txt");
	
		char* rootuser = new char[50];
		char* rootpass = new char[50];

		char** otherusers = new char*[10];
		char** otherpasses = new char*[10];

		int totalusers = 0;

		int userlimit = (rand()%600)+200;
		for (int i = 0; i <= userlimit; ++i) {

			if (totalusers < 10 && (rand()%30) == 0 && i < userlimit) {
				
				otherusers[totalusers] = new char[50];
				otherpasses[totalusers] = new char[50];

				usernames.getline(otherusers[totalusers], 50);
				passwords.getline(otherpasses[totalusers++], 50);

			}
			usernames.getline(rootuser, 50);
			passwords.getline(rootpass, 50);

		}

		// serv->SetRoot(rootuser, rootpass);
		usernames.close();
		passwords.close();

		char* hostIP = new char[15];
		sprintf(hostIP, (char*)"192.168.1.%d", (hostnum++*4));
		VirtualServer* serv = new VirtualServer(hostIP, rootuser, rootpass, otherusers, otherpasses, totalusers);
		cout << "DEBUG ROOT USER: " << rootuser << endl;
		cout << "DEBUG ROOT PASS: " << rootpass << endl;

		// usr directory with usernames and little readable notes
		Directory* usr = new Directory((char*)"usr");
		serv->AddSubdir(usr);

		Directory* rootuserdir = new Directory(serv->rootuser);

		VirtualFile* rootuserfile = new VirtualFile('t', 'a', (char*)"notetoself.txt", (char*)"Keep your password on lock\nWouldn't want anyone getting root access\n");
		rootuserdir->AddFile(rootuserfile);

		Directory* otheruserdir;
		VirtualFile* otheruserfile;
		// cout << endl << totalusers << endl;
		int rootplacement = rand()%(totalusers-1);
		for (int i = 0; i < totalusers; ++i) {

			if (i == rootplacement)
				usr->AddSubdir(rootuserdir);

			otheruserdir = new Directory(serv->otherusers[i]);
			usr->AddSubdir(otheruserdir);
			// cout << otheruserdir->name << endl;

			otheruserfile = new VirtualFile('t', 'a', (char*)"notetoself.txt", (char*)"Man, those security briefings are getting tedious\nIt's not like I have access to anything important\n");
			otheruserdir->AddFile(otheruserfile);

		}

		for (int i = 0; i < activePlayers->size(); ++i) {

			otheruserdir = new Directory(activePlayers->at(i)->DisplayName());
			usr->AddSubdir(otheruserdir);
			// cout << otheruserdir->name << endl;

			otheruserfile = new VirtualFile('t', 'a', (char*)"notefromsysadmin.txt", (char*)"Filthy, no-good hacker\nBoot off server on sight\n");
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
		serv->AddSubdir(etc);

		rootplacement = rand()%(totalusers-1);
		hash<string> strhasher;
		string temp = "";
		for (int i = 0; i < totalusers; ++i) {
			if (i == rootplacement) {
				temp += serv->rootuser;
				temp += ':';
				temp += to_string(strhasher(string(serv->rootpass)));
				temp += '\n';
			}
			temp += serv->otherusers[i];
			temp += ':';
			temp += to_string(strhasher(string(serv->otherpasses[i])));
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
		activePlayers->at(n)->cvs = servers->at(n);

	}

}

// Lists directories in cwd
void Game::LS(Player* p, char** args) {

	string temp = "\n";

	Directory* dir;
	for (vector<Directory*>::iterator i = p->cwd->subdirectories->begin(); i != p->cwd->subdirectories->end(); ++i) {
		dir = *i;
		temp += dir->name;
		temp += "  ";
	}

	VirtualFile* vir;
	for (vector<VirtualFile*>::iterator i = p->cwd->files->begin(); i != p->cwd->files->end(); ++i) {
		vir = *i;
		temp += vir->name;
		temp += "  ";
	}
	temp += '\n';

	char* toPlayer = new char[temp.size()+1];
	copy(temp.begin(), temp.end(), toPlayer);
	toPlayer[temp.size()] = '\0';

	p->SendToPlayer(toPlayer);

	free(args);

}

// Prints contents of a named file
void Game::READ(Player* p, char** args) {

	string temp = "";

	bool fileFound = false;

	char* filename = strtok(args[1], "\n");

	VirtualFile* vir;
	for (vector<VirtualFile*>::iterator i = p->cwd->files->begin(); i != p->cwd->files->end(); ++i) {
		vir = *i;
		
		if (fileFound)
			break;

		if (strcmp(vir->name, filename) == 0) {
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

	Directory* dir;
	for (vector<Directory*>::iterator i = p->cwd->subdirectories->begin(); i != p->cwd->subdirectories->end(); ++i) {
		dir = *i;
		
		if (fileFound)
			break;
		
		if (strcmp(dir->name , filename) == 0) {
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

	free(args);

}

// Executes an executable file found in <virtual-server>/bin
void Game::EXEC(Player* p, char** args) {

	char* executable = strtok(args[1], "\n");

	string temp = "";

	if (strcmp("johntheripper", executable) == 0) {
		
		bool fileFound = false;
		char* filename = strtok(args[2], "\n");
		VirtualFile* vir;
		for (vector<VirtualFile*>::iterator i = p->cwd->files->begin(); i != p->cwd->files->end(); ++i) {
			vir = *i;
			
			if (fileFound)
				break;
			
			if (strcmp(filename, vir->name) == 0) {
				
				fileFound = true;
				
				if (strcmp(filename, "passwd.txt") == 0) {
					
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
						temp += serverroot->otherpasses[i];
						temp += '\n';
					}

				} else {
					
					temp += "Format error: ";
					temp += filename;
					temp += " does not contain delineated usernames and encrypted passwords\n";
				
				}
			
			}

		}

	} else if (strcmp("pingsweep", executable) == 0) {

		temp += "SCANNING NETWORK\n";
		Directory* serv;
		for (vector<VirtualServer*>::iterator i = servers->begin(); i != servers->end(); ++i) {
			serv = *i;

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

	free(args);

}

// If credentials match server's root credentials, captures server
void Game::CAP(Player* p, char** args) {

	char* submitteduser = args[1];
	char* submittedpass = strtok(args[2], "\n");

	string temp = "";

	Directory* serverfinder = p->cwd;
	while (serverfinder->parent != NULL) {
		serverfinder = serverfinder->parent;
	}
	VirtualServer* serverroot = (VirtualServer*)serverfinder;

	if (serverroot->captured) {
		temp += "Failed to capture server: server already captured\n";
	} else if (strcmp(serverroot->rootuser, submitteduser) == 0 && strcmp(serverroot->rootpass, submittedpass) == 0) {
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

	free(args);

}

// Prints player's working directory
void Game::PWD(Player* p) {

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

	if (strcmp(destdir, "..") == 0 && p->cwd->parent != NULL) {

		p->cwd = p->cwd->parent;
		temp += "Changed directory to ";
		temp += p->cwd->name;
		temp += "\n";

	} else {

		bool fileFound = false;

		Directory* dir;
		for (vector<Directory*>::iterator i = p->cwd->subdirectories->begin(); i != p->cwd->subdirectories->end(); ++i) {
			dir = *i;
		
			if (fileFound)
				break;
			
			if (strcmp(dir->name, destdir) == 0) {
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

	free(args);

}

// Changes player's working direcotry to a new Virtual Server, if server is occupied, start competitive typing
void Game::SSH(Player* p, char** args) {

	char* destserv = strtok(args[1], "\n");

	string temp = "";
	
	if (strcmp(destserv, p->cvs->name) == 0) {

		temp += "Could not SSH to ";
		temp += destserv;
		temp += ": already connected to ";
		temp += destserv;
		temp += '\n';

		char* toPlayer = new char[temp.size()+1];
		copy(temp.begin(), temp.end(), toPlayer);
		toPlayer[temp.size()] = '\0';

		p->SendToPlayer(toPlayer);

		free(args);
		return;
	}


	bool serverFound = false;

	VirtualServer* serv;
	for (vector<VirtualServer*>::iterator i = servers->begin(); i != servers->end(); ++i) {
		serv = *i;

		if (serverFound)
			break;

		if (strcmp(serv->name, destserv) == 0) {

			serverFound = true;

			int playersInDestServer = 0;
			Player* hacker;
			for (vector<Player*>::iterator i = activePlayers->begin(); i != activePlayers->end(); ++i) {
				hacker = *i;
				if (strcmp(hacker->cvs->name, serv->name) == 0) {
					playersInDestServer++;
				}
			}

			if (playersInDestServer >= 2) {
				temp += "Could not SSH to ";
				temp += serv->name;
				temp += ": Server overloaded\n";
			} else if (playersInDestServer == 1) {

				// Kick off Competitive typing game here, determine who gets to stay and who gets booted.

			} else {

				p->cwd = serv;
				p->cvs = serv;
				temp += "SSH to ";
				temp += serv->name;
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

	free(args);

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