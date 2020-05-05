#ifndef PLAYER_H
#define PLAYER_H
class VirtualServer; //Forward declaration so the below doesn't error
class Player {
//TODO: Make everything private lol
public:
    Player();
    int socket;

    int score;
    std::string username;
    VirtualServer* currentServer; //TODO: Test if this compile errors since both are dependent on each other
};
#endif