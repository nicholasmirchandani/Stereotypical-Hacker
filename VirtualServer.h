#ifndef VIRTUAL_SERVER_H
#define VIRTUAL_SERVER_H
#include "Player.h"
#include <map>
class VirtualServer {
public:
    VirtualServer();

    std::string ip; //Stored as a string for easy comparison for the server
    //Root User and pass are separate strings to make verification easier
    std::string rootUser;
    std::string rootPass;
    std::map<std::string, std::string> users; //Using a map for username and password to keep them tied to the same user since it's in std

    bool captured;
    Player* currentPlayer;
};
#endif