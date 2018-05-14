#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "WindowManager.h"
#include "Model.h"
#include "TimeManager.h"
#include "Camera.h"
#include "InputManager.h"
#include "..\AssetFactory\GameAssetFactory.h"
#include "GameWorld.h"
#include "..\headers\ScriptManager.h"
#include "TextureManager.h"

//#include "..\AssetFactory\Bruteforce.h"

//#include "PhysicsEngine.h"

//#pragma comment(lib, "legacy_stdio_definitions.lib")

	/*
	* @class GameControlEngine
	* @brief The main game engine class
	*
	* This class encompasses all the components of our game engine. It handles the controller
	* aspect including physics, input and resource storing / loading.
	*
	* @author Cordell Smith
	* @author Jack Matters
	* @author Michael Bell
	* @version 01
	* @date 29/03/2018
	*
	*/
class GameControlEngine
{
public:
		/**
		* @brief Default constructor
		*
		* Empty default constructor.
		*
		* @return null
		*/
	GameControlEngine() { }
	
		/**
		* @brief Destructor
		*
		* Destructor calls Destroy() that cleans up and frees up memory.
		*
		* @return null
		*/
	~GameControlEngine() { Destroy(); }

		/**
		* @brief Runs the game engine
		*
		* This is the main function for our application, which is called by the main() function
		* in the window manager to abstract the game engines entry point. It initializes all systems,
		* runs the main game loop then destroys to free up memory when the user exits.
		*
		* @return const int
		*/
	const int RunEngine();

		/**
		* @brief Gets the window manager object
		*
		* Returns the current window manager.
		*
		* @return WindowManager*
		*/
	WindowManager* GetWindowManager() { return m_windowManager; }

		/**
		* @brief Gets the window manager object
		*
		* This sets the window manager for the application allowing the OpenGL context and window
		* properties to be passed to into the game engine for use.
		*
		* @param WindowManager* windowManager
		* @return void
		*/
	void SetWindowManager(WindowManager* windowManager) { m_windowManager = windowManager; }

		/**
		* @brief Gets the camera object
		*
		* Returns the current camera object.
		*
		* @return Camera*
		*/
	Camera* GetCamera() { return m_camera; }

		/**
		* @brief Sets the camera object
		*
		* Sets the current camera object to the parameter given.
		*
		* @param Camera* camera
		* @return void
		*/
	void SetCamera(Camera* camera) { m_camera = camera; }

		/**
		* @brief Initializes the engine
		*
		* This function handles loading any required system that the engine uses
		* and loading any files into their respected data structures for further use.
		*
		* @return void
		*/
	void Initialize();

		/**
		* @brief Main game loop
		*
		* This function runs the main game loop that is updated every frame and will continue
		* until the user quits the program.
		*
		* @return void
		*/
	void GameLoop();

	void InitializePhysics();

		/**
		* @brief Memory management
		*
		* This function runs when the game loop has finished and handles freeing up any memory
		* that was used.
		*
		* @return void
		*/
	void Destroy();

protected:

	/// Screen width
	static const int ScreenWidth = 1280;

	/// Screen height
	static const int ScreenHeight = 720;

	/// Engines window object
	WindowManager* m_windowManager;
	
	/// Engines camera object
	Camera* m_camera;

	/// Game asset factory object
	GameAssetFactory* m_assetFactory;

	/// Physics world
	PhysicsEngine* m_physicsWorld;

	/// Vector of all collision objects (static and dynamic)
	std::vector<btVector3> m_collisionBodyPos;

	/// Game world object
	GameWorld* m_gameWorld;

	/// Terrain
	// Needs to be moved, should be created in init and passed to gameworld without being here
	Bruteforce bfTerrain;
};