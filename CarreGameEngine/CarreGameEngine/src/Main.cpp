#include "GL/glew.h"									// Include the GLEW library to manage OpenGL extensions

#include "..\headers\GameWorld.h"

GLApplication gameWorld;

int startGame()
{
	// This calls our Initialize() function below which creates the window and triangle
	gameWorld.Initialize();

	// This is our main game loop which will run until we close the window or hit Escape.
	gameWorld.GameLoop();

	// Once we hit Escape this will clean up the application's resources.
	gameWorld.Destroy();

	// Return 0 for success
	return 0;
}




