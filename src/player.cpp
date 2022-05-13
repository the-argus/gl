#include "player.h"


Player::Player()
{
    // starting conditions for player
    Player::pos = glm::vec3(0.0f, 0.0f, 3.0f);
    Player::dir = glm::vec3(0.0f, 0.0f, -1.0f);
    Player::speed = 0.3f;
    Player::up = glm::vec3(0.0f, 0.1f, 0.0f);
}

void Player::Move(glm::vec3 delta)
{
    Player::pos += delta;
}

