#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "Camera.h"
#include "Model.h"
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>					
#include <GLM\gtx\transform2.hpp>
#include "PhysicsEngine.h"

//#pragma comment(lib, "legacy_stdio_definitions.lib")

/// Struct to hold both vertex and fragment shaders (needs to be moved)
struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

	/*
	* @class GameWorld
	* @brief The instance of our gameworld
	*
	* Contains all the necessary objects, terrains, characters and models that populate the
	* world that will be the view to the user.
	*
	* @note Currently not entirely implemented.
	*
	* @author Cordell Smith
	* @version 01
	* @date 29/03/2018 CS
	*
	*/
class GameWorld
{
public:
		/**
		* @brief Default constructor
		*
		* Empty default constructor.
		*
		* @return null
		*/
	GameWorld() { }

		/**
		* @brief Destructor
		*
		* Empty destructor.
		*
		* @return null
		*/
	~GameWorld() { }

	void Init();

	void Update();

	void Destroy();

	void PrepareTestModel(const char* filePath, int& modelVertexSize);
	void PrepareColourPanel();

	void SetCamera(Camera* camera) { m_camera = camera; }
	Camera* GetCamera() { return m_camera; }

	/**
	* @brief Initialize all physics
	*
	* This function creates all rigid bodies for every game object, and adds their locations to a vector for drawing/updating
	*
	* @return null
	*/
	void InitializePhysics();

	/**
	* @brief Updates all physics
	*
	* This function updates all physics bodies, simulating them each step
	*
	* @return null
	*/
	void UpdatePhysics();

protected:
	Model m_testModel;
	Model m_colourPanel;
	int m_modelVertexSize;
	ShaderSource m_shaderSource1;
	ShaderSource m_shaderSource2;

	Camera* m_camera;

	/// Physics world
	PhysicsEngine m_physicsWorld;

	/// Vector of all collision objects (static and dynamic)
	std::vector<btVector3> m_collisionBodyPos;
};