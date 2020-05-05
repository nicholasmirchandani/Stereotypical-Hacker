#ifndef PLAYER_H
#define PLAYER_H
class Player {
//TODO: Make everything private lol
public:
    int socket;

    int score;
    std::string username;
    VirtualServer* currentServer; //TODO: Test if this compile errors since both are dependent on each other
};
#endif