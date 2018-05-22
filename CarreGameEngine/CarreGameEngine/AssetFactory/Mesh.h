#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <GLM\glm.hpp>									// Used for the GLM math library
#include <GLM\gtc\matrix_transform.hpp>					
#include <GLM\gtx\transform2.hpp>						
#include <GL\glew.h>

#include "..\headers\Shader.h"
#include "..\headers\Camera.h"

struct Vertex3
{
	glm::vec3 m_position; // The x,y,z position
	glm::vec4 m_colour; // Colour r,g,b,a
	glm::vec3 m_normal; // The normal coordinates
	glm::vec2 m_texCoords; // The uv coordinates
	glm::vec3 m_tangent;
	glm::vec3 m_biTangent;
};

struct Texture
{
	unsigned int m_id;
	std::string m_type;
	std::string m_path;
};

class Mesh
{
public:
	Mesh();
	~Mesh() { }

	Mesh(std::vector<Vertex3> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void SetupMesh();
	void Draw(Shader* shader);

		/**
		* @brief Sets the camera object
		*
		* Sets the camera object to the world camera object to retrieve the view and projection
		* matrices.
		*
		* @param Camera* camera
		* @return void
		*/
	void SetCamera(Camera* camera) { m_camera = camera; }

	void SetPosition(glm::vec3 position) { m_position = position; }
	void SetRotation(glm::vec3 rotation) { m_rotation = rotation; }
	void SetScale(glm::vec3 scale) { m_scale = scale; }

	std::vector<Vertex3>& GetVertices() { return m_vertices; }
	void SetVertices(std::vector<Vertex3> vertices) { m_vertices = vertices; }

	std::vector<Texture>& GetTextures() { return m_textures; }
	
	void CreateTransformationMatrix(glm::mat4& modelMatrix);
		
protected:
	std::vector<Vertex3> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;

	unsigned int VAO, VBO, EBO;

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	Camera* m_camera;
};