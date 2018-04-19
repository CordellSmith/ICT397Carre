		/**
		* @class Mesh
		* @brief 
		*
		*
		* @author
		* @version 01
		* @date 29/03/2018
		*
		*/
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


		/**
		* @brief Vertex 3 struct containing x,y,z positions,
		* normal coordinates and uv coordinates
		*/
struct Vertex3
{
	glm::vec3 m_position; // The x,y,z position
	glm::vec3 m_normal; // The normal coordinates
	glm::vec2 m_texCoords; // The uv coordinates
};

class Mesh
{
public:
		/**
		* @brief Default constructor
		*
		* @return null
		*/
	Mesh() { }
		/**
		* @brief Destructor
		*
		* This function is required for the inheriting class to have its destructor called.
		*
		* @return null
		*/
	~Mesh() { }

		/**
		* @brief Mesh
		* @param std::vector<Vertex3> vertices
		* @param std::vector<unsigned int> indices
		*
		* @return Mesh
		*/
	Mesh(std::vector<Vertex3> vertices, std::vector<unsigned int> indices);

		/**
		* @brief SetupMesh
		*
		* @return void
		*/
	void SetupMesh();
		/**
		* @brief Draw
		*
		* @return void
		*/
	void Draw();

protected:
	std::vector<Vertex3> m_vertices;
	std::vector<unsigned int> m_indices;
	//std::vector<Texture> m_textures;

	unsigned int VAO, VBO, EBO;

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
};