#pragma once

#include <glm/glm.hpp>

struct Player
{
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 up;

    float speed;

    Player();

    void Move(glm::vec3 delta);
};
