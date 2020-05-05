#ifndef VIRTUAL_SERVER_H
#define VIRTUAL_SERVER_H
#include "Player.h"
class VirtualServer {
    //Root User and pass are separate strings to make verification easier
    std::string rootUser;
    std::string rootPass;
    std::map<std::string, std::string> users; //Using a map for username and password to keep them tied to the same user since it's in std
};

bool captured;
Player* currentPlayer;
#endif