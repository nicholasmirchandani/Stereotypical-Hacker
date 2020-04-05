#include "Player.h"

Player::Player(SocketConnection, bool isHost, string displayName)
{
  this.isHost = isHost;
  this.displayName = displayName;
}

Player::~Player()
{

}

Player::changeName(string newName)
{
  displayName = newName;
}
