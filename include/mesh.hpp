#pragma once
#include <vector>
#include "glstructs.hpp"
#include "shader.hpp"

class Mesh
{
  public:
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
		 vector<Texture> textures);
	void Draw(Shader &shader);

  private:
	//  render data
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};
