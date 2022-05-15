#include "player.h"


Player::Player()
{
    // starting conditions for player
    Player::dir = glm::vec3(0.0f, 0.0f, -1.0f);
    pitch = 0.0f;
    yaw = -90.0f;
    
    Player::pos = glm::vec3(0.0f, 0.0f, 3.0f);
    Player::Rotate(pitch, yaw); // initial rotation
    Player::speed = 10.0f;
    Player::up = glm::vec3(0.0f, 0.1f, 0.0f);
}

void Player::Move(glm::vec3 delta)
{
    Player::pos += delta;
}

void Player::Rotate(float pitch, float yaw)
{
    Player::yaw += yaw;
    Player::pitch += pitch;
    
    // restrict pitch
    if(Player::pitch > 89.0f)
        Player::pitch = 89.0f;
    if(Player::pitch < -89.0f)
        Player::pitch = -89.0f;

    Player::dir.x = cos(glm::radians(Player::yaw)) 
        * cos(glm::radians(Player::pitch));
    Player::dir.y = sin(glm::radians(Player::pitch));
    Player::dir.z = sin(glm::radians(Player::yaw))
        * cos(glm::radians(Player::pitch));
}

glm::vec3 Player::getDir()
{
    return Player::dir;
}
