#pragma once

#include <glm/glm.hpp>

struct Player
{
private:
    glm::vec3 dir;

    float yaw;
    float pitch;
public:
    glm::vec3 pos;
    glm::vec3 up;

    float speed;

    Player();

    void Move(glm::vec3 delta);
    void Rotate(float pitch, float yaw);
    glm::vec3 getDir();
};
