#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

#include "Camera.h"
#include "Model.h"
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>					
#include <GLM\gtx\transform2.hpp>
#include "PhysicsEngine.h"
#include "..\AssetFactory\IGameObject.h"
#include "..\ImageDB\stb_image.h"

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
	* @version 01
	* @date 29/03/2018
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

		/**
		* @brief Initialises the game world
		*
		* Sets up all the assets of the game world.
		*
		* @return void
		*/
	void Init(std::multimap<OBJ_TYPE, IGameObject*> gameAssets);

		/**
		* @brief Updates the game world
		*
		* Recieves any changes that have been made due to input, game logic or
		* physics and updates the current game instance.
		*
		* @return void
		*/
	void Update();

		/**
		* @brief Destroys the game world
		*
		* Anything that was created within this current game world context is
		* destroyed or deleted to free up memory.
		*
		* @return void
		*/
	void Destroy();

		/**
		* @brief Prepares the test model
		*
		* Used to prepare the test model to be drawn to the screen.
		*
		* @param const char* filePath
		* @param int& modelVertexSize
		* @return void
		*/
	void PrepareTestModel(const char* filePath, int& modelVertexSize);

		/**
		* @brief Prepares the colour panel
		*
		* Used to prepare the test model to be drawn to the screen.
		*
		* @return void
		*/
	void PrepareColourPanel();

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
		* @param Camera* camera
		* @return void
		*/
	void SetCamera(Camera* camera) { m_camera = camera; }

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

	void SetGameAssets(std::multimap<OBJ_TYPE, IGameObject*> gameAssets) { m_gameAssets = gameAssets; }

protected:
	/// Models to load
	Model m_colourPanel, m_testModel;
	int m_modelVertexSize;

	/// Shader sources
	ShaderSource m_assimpShaderSource, m_shaderSource1, m_shaderSource2;

	/// Camera object
	Camera* m_camera;

	/// Physics world
	PhysicsEngine m_physicsWorld;

	/// Vector of all collision objects (static and dynamic)
	std::vector<btVector3> m_collisionBodyPos;

	std::multimap<OBJ_TYPE, IGameObject*> m_gameAssets;
};