#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>					
#include <GLM\gtx\transform2.hpp>

#include "Camera.h"
#include "PhysicsEngine.h"
#include "TextureManager.h"
#include "ScriptManager.h"
#include "TimeManager.h"
#include "..\AssetFactory\Model.h"
#include "..\AssetFactory\IGameAsset.h"
#include "..\AssetFactory\Bruteforce.h"
#include "..\AssetFactory\Player.h"
#include "..\Renderer\OpenGl.h"
#include "..\headers\Shader.h"

//#pragma comment(lib, "legacy_stdio_definitions.lib")

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
	~GameWorld() { Destroy(); }

		/**
		* @brief Initialises the game world
		*
		* Sets up all the assets of the game world.
		*
		* @return void
		*/
	void Init(Player* player, std::multimap<std::string, IGameAsset*> gameAssets);

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
		* @brief Sets the physics world properties
		*
		* Passes the initialized physics engine and populated collision bodies to the game world
		* object to be used.
		*
		* @param PhysicsEngine* physicsEngine
		* @param std::vector<btVector3> collisionBodies
		* @return void
		*/
	void SetPhysicsWorld(PhysicsEngine* physicsEngine, std::vector<btVector3> collisionBodies);

		/**
		* @brief Load all textures
		*
		* This function loads all required textures into the TextureManager
		*
		* @return null
		*/
	void LoadAllTextures();

		/**
		* @brief Updates all physics
		*
		* This function updates all physics bodies, simulating them each step
		*
		* @return null
		*/
	void UpdatePhysics();

	void SetGameAssets(std::multimap<std::string, IGameAsset*> gameAssets) { m_gameAssets = gameAssets; }

	void SetTerrains(std::vector<Bruteforce*> terrains) { m_terrains = terrains; }

protected:
	/// Shader sources
	ShaderSource m_assimpShaderSource, m_shaderSource1, m_shaderSource2, m_testShaderSource;

	/// Camera object
	Camera* m_camera;

	/// Physics world
	PhysicsEngine* m_physicsWorld;

	/// Vector of all collision objects (static and dynamic)
	std::vector<btVector3> m_collisionBodyPos;

	std::multimap<std::string, IGameAsset*> m_gameAssets;

	std::vector<Bruteforce*> m_terrains;

	Player* m_player;
	float s = 0.01;

	OpenGl m_glRenderer;
};