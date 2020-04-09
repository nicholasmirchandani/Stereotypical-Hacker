#include "Player.h"

Player::Player(SocketConnection client, bool isHost, string displayName)
{
  this.isHost = isHost;
  this.displayName = displayName;
  socket = client;
}

Player::~Player()
{

}

Player::changeName(string newName)
{
  displayName = newName;
}
