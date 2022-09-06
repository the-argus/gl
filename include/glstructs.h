#pragma once

#include <glad/glad.h> // include glad to get the required OpenGL headers
#include <glm/gtc/type_ptr.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	unsigned int id;
    std::string type;
};
