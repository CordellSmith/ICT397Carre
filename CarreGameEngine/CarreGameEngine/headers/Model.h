#pragma once

/// Make sure GLM is using radians instead of degrees
#define GLM_FORCE_RADIANS							

#include <stdio.h>										
#include <stdlib.h>										
#include <GL\glew.h>									
#include <GLM\glm.hpp>									
#include <GLM\gtc\matrix_transform.hpp>					
#include <GLM\gtx\transform2.hpp>

#include "Shader.h"										
#include "Camera.h"										

	/**
	* @struct Vertex
	* @brief Basic vertex struct
	*
	* This vertex struct holds the information about a vertex including position (x,y,z),
	* colour (rgba), texture coordinates (uv) and normals (normal).
	*
	* @author Cordell Smith
	* @version 01
	* @date 29/03/2018 CS
	*
	*/
struct Vertex
{
	/// The x,y,z position
	glm::vec3 xyz;
	/// Colour value
	glm::vec4 rgba;	
	/// Texture coordinates
	glm::vec2 uv;
	/// Normal coordinates
	glm::vec3 normal;
};

	/**
	* @class Model
	* @brief Model class to create a 3D model
	*
	* This class is used to store all model information including initializion of the model 
	* to be rendered and drawn onto the screen using shaders.
	* 
	* @author Cordell Smith
	* @version 01
	* @date 29/03/2018 CS
	*
	*/
class Model
{
public:
		/**
		* @brief Default constructor
		*
		* This is the default constuctor that sets the scale value of the model to 1.
		*
		* @return null
		*/
	Model() { m_scale = glm::vec3(1.0f, 1.0f, 1.0f); }
		
		/**
		* @brief Destructor
		*
		* This is the destructor that calls Destroy() to free up the used memory.
		*
		* @return null
		*/
	~Model() { Destroy(); }

		/**
		* @brief Initializes the model
		*
		* This function takes a list of x, y, z vertices and its length to create a vertex buffer object (VBO)
		* and vertex array object (VBO) as well as the corresponding vertex and fragment shaders as strings.
		*
		* @param Vertex vertices[]
		* @param int length
		* @param std::string vertShaderString
		* @param std::string fragShaderString
		* @return void
		*/
	void Initialize(Vertex vertices[], int length, std::string vertShaderString, std::string fragShaderString);
	
		/**
		* @brief Renders the model
		*
		* This function draws the model to the backbuffer by first initializing the shader, turning it on and
		* binding the vertex array. Each vertex attribute is enabled and glDrawArrays() function is called to 
		* draw the vertices to the screen. The attributes are disabled and the shader is then turned off.
		*
		* @return void
		*/
	void Render();

		/**
		* @brief Gets the models position
		*
		* Returns the position of the model as a glm::vec3.
		*
		* @return glm::vec3
		*/
	glm::vec3 GetPosition() { return m_positon; }

		/**
		* @brief Sets the models position
		*
		* Sets the position of the model using the parameter given.
		*
		* @param glm::vec3 position
		* @return void
		*/
	void SetPosition(glm::vec3 position) { m_positon = position; }

		/**
		* @brief Gets the models rotation
		*
		* Returns the rotation of the model as a glm::vec3.
		*
		* @return glm::vec3
		*/
	glm::vec3 GetRotation() { return m_rotation; }

		/**
		* @brief Sets the models rotation
		*
		* Sets the rotation of the model using the parameter given.
		*
		* @param glm::vec3 rotation
		* @return void
		*/
	void SetRotation(glm::vec3 rotation) { m_rotation = rotation; }

		/**
		* @brief Gets the models scale
		*
		* Returns the scale of the model as a glm::vec3.
		*
		* @return glm::vec3
		*/
	glm::vec3 GetScale() { return m_scale; }

		/**
		* @brief Sets the models scale
		*
		* Sets the rotation of the model using the parameter given.
		*
		* @param glm::vec3 scale
		* @return void
		*/
	void SetScale(glm::vec3 scale) { m_scale = scale; }

		/**
		* @brief Gets the camera object
		*
		* Returns a pointer to the camera object to access the view and projection matrices.
		*
		* @return Camera*
		*/
	Camera* GetCamera() { return m_camera; }

		/**
		* @brief Sets the camera object
		*
		* Sets the camera object to the world camera object to retrieve the view and projection
		* matrices.
		*
		* @param Camera* pCamera
		* @return void
		*/
	void SetCamera(Camera* pCamera) { m_camera = pCamera; }

		/**
		* @brief Cleans up the VAO and VBOs
		*
		* Unbinds any VAO and VBOs and frees up any memory used.
		*
		* @return void
		*/
	void Destroy();

protected:
	/// These are used as pointers to each index where each information is stored within the vertex buffer object
	static const GLuint m_vertexIndex = 0;
	static const GLuint m_colorIndex = 1;
	static const GLuint m_textureIndex = 2;
	static const GLuint m_normalIndex = 3;

	Vertex* m_vertices;
	GLint m_verticesLength;
	/// VBO id
	GLuint m_VBO;
	/// VAO id
	GLuint m_VAO;

	/// The model's position
	glm::vec3 m_positon;
	/// The model's rotation
	glm::vec3 m_rotation;
	/// The model's scale
	glm::vec3 m_scale;											
	
	/// Shader object
	Shader m_shader;

	/// Camera object
	Camera* m_camera;
};