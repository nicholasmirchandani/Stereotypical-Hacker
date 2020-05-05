#include "VirtualServer.h"

VirtualServer::VirtualServer() {
    //Default Constructor
    //Only setting these 2 attributes because all the others vary from server to server and are setup as needed
    bool captured = false;
    Player* currentPlayer = nullptr;
}

bool VirtualServer::captureServer(std::string username, std::string password) {
    if(username == rootUser && password == rootPass) {
        captured = true;
        return true;
    }
    return false;
}