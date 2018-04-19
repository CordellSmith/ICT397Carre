#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <GLM\glm.hpp>									// Used for the GLM math library
#include <GLM\gtc\matrix_transform.hpp>					
#include <GLM\gtx\transform2.hpp>						
#include <GL\glew.h>

//#include "Texture.h"
#include "..\headers\Shader.h"

struct Vertex3
{
	glm::vec3 m_position; // The x,y,z position
	glm::vec3 m_normal; // The normal coordinates
	glm::vec2 m_texCoords; // The uv coordinates
};

class Mesh
{
public:
	Mesh() { }
	~Mesh() { }

	Mesh(std::vector<Vertex3> vertices, std::vector<unsigned int> indices);

	void SetupMesh();
	void Draw();

protected:
	std::vector<Vertex3> m_vertices;
	std::vector<unsigned int> m_indices;
	//std::vector<Texture> m_textures;

	unsigned int VAO, VBO, EBO;

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_m_scale;
};