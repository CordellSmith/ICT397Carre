		/*
		*
		* @class GameControlEngine
		* @brief	Class for initializing the game world, controlling objects, the main 
		*			game loop and responsible for cleanup.
		*
		* @author
		* @date 29/04/2018
		* @version 1.0
		*/
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "WindowManager.h"					// Include our WindowManager to handle the cross-platform window
#include "Model.h"							// Include our Model class to draw the setup and draw the triangle
#include "TimeManager.h"					// Include our TimerManager class for our time slices
#include "Camera.h"							// Include our Camera class for our camera management
#include "InputManager.h"					// Include our InputManager class definition
#include "..\AssetFactory\GameAssetFactory.h"
#include "GameWorld.h"


#include "PhysicsEngine.h"					// Physics Engine
#include "..\..\Dependencies\glut\include\glut.h"	// Do we need this?

#include "..\Resource\ResourceFactory.h"

//#pragma comment(lib, "legacy_stdio_definitions.lib")

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};
		/**
		* @brief Game Control Engine class
		*
		* This is our main application class that handles orchestrating the init, game loop and cleanup
		*/
class GameControlEngine
{
public:

	static const int ScreenWidth = 1280;
	static const int ScreenHeight = 720;

		/**
		* @brief Default constructor
		*
		* @return null
		*/
	GameControlEngine() {}

		/**
		* @brief Destructor with clean up function
		* 
		* @return null
		*/
	~GameControlEngine() { Destroy(); }

		/**
		* @brief	This is the main function for our application, which is called by the main() function
		*			in the WindowManager.cpp file.  This helps abstract out the environments entry point.
		*
		* @return int
		*/
	int GLMain();

		/**
		* @brief	This gets the window manager for the application.
		*
		* @return WindowManager
		*/
	WindowManager *GetWindowManager() { return WindowManager; }
		/**
		* @brief	This sets the window manager for the application.  This should be set right
		*			after the GameControlEngine is created in the main() function.
		* @param WindowManager *pWindowManager
		*
		* @return void
		*/
	void SetWindowManager(WindowManager *pWindowManager) { WindowManager = pWindowManager; }

		/**
		* @brief	Returns the camera for our application
		*
		* @return Camera
		*/
	Camera *GetCamera() { return Camera; }
		/**
		* @brief	Sets the camera for our application
		* @param	Camera *pCamera
		*
		* @return void
		*/
	void SetCamera(Camera *pCamera) { Camera = pCamera; }

		/**
		* @brief	This initializes the application
		*
		* @return void
		*/
	void Initialize();

		/**
		* @brief	This runs the game loop and only quits when the user hits Escape or closes the window
		*
		* @return void
		*/
	void GameLoop();

		/**
		* @brief	This cleans up the application and tells other objects to also cleanup
		*
		* @return void
		*/
	void Destroy();

	ShaderProgramSource GameControlEngine::ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

protected:

		/**
		* @brief	 Our abstract window manager to create the window and manage input, which should be
		* environment agnostic.  Our main application shouldn't know if it's Win32 or GLFW.
		* This is instantiated and set in GLFWManager.cpp in the main() function.
		*/
	WindowManager *WindowManager;

		/**
		* @brief	Add our camera object to our application
		*/
	Camera *Camera;


		/**
		* @brief	Physics world
		*/
	PhysicsEngine physicsWorld;

		/**
		* @brief	Array of collision body positions
		*/
	std::vector<btVector3> collisionBodyPos;
		/**
		* @brief	Used to draw spheres for testing
		*/
	GLUquadricObj* quad;

		/**
		* @brief	Game asset factory
		*/
	GameAssetFactory m_assetFactory;

		/**
		* @brief	Our game world instance (singleton this later maybe?)
		*/
	GameWorld m_gameWorld;
};