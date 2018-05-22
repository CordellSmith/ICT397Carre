#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <GLM\glm.hpp>									// Used for the GLM math library
#include <GLM\gtc\matrix_transform.hpp>					
#include <GLM\gtx\transform2.hpp>						
#include <GL\glew.h>

#include "..\Common\Vertex3.h"
#include "..\headers\Shader.h"

struct Texture
{
	unsigned int m_id;
	std::string m_type;
	std::string m_path;
};

class Mesh
{
public:
	Mesh() { }
	~Mesh() { }

	Mesh(std::vector<Vertex3> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void SetPosition(glm::vec3 position) { m_position = position; }
	void SetRotation(glm::vec3 rotation) { m_rotation = rotation; }
	void SetScale(glm::vec3 scale) { m_scale = scale; }

	std::vector<Vertex3>& GetVertices() { return m_vertices; }
	void SetVertices(std::vector<Vertex3> vertices) { m_vertices = vertices; }

	std::vector<unsigned int>& GetIndices() { return m_indices; }
	void SetIndices(std::vector<unsigned int> indices) { m_indices = indices; }

	std::vector<Texture>& GetTextures() { return m_textures; }
		
	unsigned int VAO, VBO, EBO;
protected:
	std::vector<Vertex3> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
};