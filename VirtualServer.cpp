#include "VirtualServer.h"
#include <iostream> //DEBUG ONLY

VirtualServer::VirtualServer() {
    //Default Constructor
    //Only setting these 2 attributes because all the others vary from server to server and are setup as needed
    bool captured = false;
    Player* currentPlayer = nullptr;
}

bool VirtualServer::captureServer(std::string username, std::string password) {
    std::cout << username << rootUser << password << rootPass << std::endl;
    std::cout << "ROOT USER is USERNAME: " << (username.compare(rootUser) == 0) << std::endl;
    std::cout << "ROOT PASSWORD IS PASSWORD: " << (password.compare(rootPass) == 0) << std::endl;
    if((username.compare(rootUser) == 0) && (password.compare(rootPass) == 0)) {
        captured = true;
        return true;
    }
    return false;
}