		/**
		* @class Model
		* @brief 
		*
		* The Model class to handle initializing, rendering and destroying of a 3D object
		*
		* @author
		* @version 01
		* @date 29/03/2018 
		*
		*/
#pragma once

#define GLM_FORCE_RADIANS								// Make sure GLM is using radians instead of degrees

#include <stdio.h>										// Used for fprintf and stderr
#include <stdlib.h>										// Used for the exit() function
#include <GL\glew.h>									// Used for the OpenGL types like GLuint
#include <GLM\glm.hpp>									// Used for the GLM math library
#include <GLM\gtc\matrix_transform.hpp>					// Used for the GLM math library
#include <GLM\gtx\transform2.hpp>						// Used for the GLM math library
#include "Shader.h"										// Used for loading and running our shaders
#include "Camera.h"										// Used to get access to the view and projection matrices
using namespace glm;


		/**
		* @brief This is our custom struct to store 
		* attributes for a vertex like the x,y,z values.
		*/
struct Vertex
{
	vec3 xyz;											// The x,y,z position
	vec4 rgba;											// The r,g,b,a (Red, Green, Blue, Alpha) color values
	vec2 uv;											// The uv coordinates
	vec3 normal;										// The normal coordinates
};

		/**
		* @brief The Model class to handle initializing, 
		* rendering and destroying of a 3D object
		*/
class Model
{
public:

	// These are created to standardize the index properties used for vertex properties.
	static const GLuint kVertexIndex = 0;				// This is for the x,y,z vertices
	static const GLuint kColorIndex = 1;				// This is for the r,g,b,a colors
	static const GLuint kTextureIndex = 2;				// This is for the u,v texture coordinates
	static const GLuint kNormalIndex = 3;				// This is for the x,y,z normals

	
		/**
		* @brief Default constructor
		* Set the scale of our model to 100% by default
		* @param vec3 Scale
		* 
		* @return null
		*/ 
	Model()		{ Scale = vec3(1.0f, 1.0f, 1.0f); }
		/**
		* @brief Destructor
		*
		* @return null
		*/
	~Model()	{ Destroy(); }

		/**
		* @brief Initialize function
		* Takes in a list of x,y,z vertices and vertex length to create the VBO and VAO's from,
		* as well as the text file locations for the vertex and fragment shaders to load.
		*
		* @param Vertex vertices[]
		* @param int length
		* @param std::string strVertex
		* @param std::string strFragment
		*
		* @return void
		*/
	void Initialize(Vertex vertices[], int length, std::string strVertex, std::string strFragment);
	
		/**
		* @brief This draws the Model to the backbuffer
		*
		* @return void
		*/
	void Render();

		/**
		* @brief Create the getter for the model's position
		*
		* @return vec3
		*/
	vec3 GetPosition() { return Position; }
		/**
		* @brief Create the setters for the model's position
		* @param vec3 position
		*
		* @return void
		*/
	void SetPosition(vec3 position) { Position = position; }

		/**
		* @brief Create the setters for the model's rotation
		*
		* @return vec3
		*/
	vec3 GetRotation() { return Rotation; }
		/**
		* @brief Create the setters for the model's rotation
		* @param vec3 rotation
		*
		* @return void
		*/
	void SetRotation(vec3 rotation) { Rotation = rotation; }

		/**
		* @brief Create the setters for the model's scale
		*
		* @return vec3
		*/
	vec3 GetScale() { return Scale; }
		/**
		* @brief Create the getters for the model's scale
		* @param vec3 scale
		*
		* @return void
		*/
	void SetScale(vec3 scale) { Scale = scale; }

		/**
		* @brief This gets the camera to get 
		* access to the view and projection matrices
		*
		* @return Camera
		*/
	Camera *GetCamera() { return Camera; }
		/**
		* @brief This sets the camera to get
		* access to the view and projection matrices
		* @param Camera *pCamera
		*
		* @return void
		*/
	void SetCamera(Camera *pCamera) { Camera = pCamera; }

		/**
		* @brief Destructor function to clean up
		*
		* @return void
		*/
	void Destroy();

protected:

	Vertex* Vertices;									// The list of the vertices
	GLint VerticesLength;								// The length of the vertices
	GLuint VertexBufferId;								// The VBO Id stored by the VAO
	GLuint VertexArrayObjectId;							// The VAO Id to bind when drawing
	
	vec3 Position;										// The model's position
	vec3 Rotation;										// The model's rotation
	vec3 Scale;											// The model's scale
	
	// Our shader object to initialize and run the shaders
	Shader Shader;

	// Add our camera object to access its matrices
	Camera *Camera;
};