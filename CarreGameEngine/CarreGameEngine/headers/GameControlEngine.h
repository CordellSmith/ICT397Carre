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

// This is our main application class that handles orchestrating the init, game loop and cleanup
class GameControlEngine
{
public:

	// The width and height of the application
	static const int ScreenWidth = 1280;
	static const int ScreenHeight = 720;

	// Create a default constructor and deconstructor which cleans up
	GameControlEngine() {}
	~GameControlEngine() { Destroy(); }

	// This is the main function for our application, which is called by the main() function
	// in the WindowManager.cpp file.  This helps abstract out the environments entry point.
	int GLMain();

	// This gets and sets the window manager for the application.  This should be set right
	// after the GameControlEngine is created in the main() function.
	WindowManager *GetWindowManager() { return WindowManager; }
	void SetWindowManager(WindowManager *pWindowManager) { WindowManager = pWindowManager; }

	// We now pass in a Camera object to our application
	Camera *GetCamera() { return Camera; }
	void SetCamera(Camera *pCamera) { Camera = pCamera; }

	// This initializes the application
	void Initialize();

	// This runs the game loop and only quits when the user hits Escape or closes the window
	void GameLoop();

	// This cleans up the application and tells other objects to also cleanup
	void Destroy();

	ShaderProgramSource GameControlEngine::ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

protected:

	// Our abstract window manager to create the window and manage input, which should be
	// environment agnostic.  Our main application shouldn't know if it's Win32 or GLFW.
	// This is instantiated and set in GLFWManager.cpp in the main() function.
	WindowManager *WindowManager;

	// Add our camera object to our application
	Camera *Camera;

	// Physics world
	PhysicsEngine physicsWorld;
	// Array of collision body positions
	std::vector<btVector3> collisionBodyPos;
	// Used to draw spheres for testing
	GLUquadricObj* quad;

	// Game asset factory
	GameAssetFactory m_assetFactory;

	// Our game world instance (singleton this later maybe?)
	GameWorld m_gameWorld;
};