#pragma once

#include <string>										// Used for our STL string objects
#include <fstream>										// Used for fprintf and stderr									
#include "GLFW\glfw3.h"									// Used to reference the GLFW library
#include "WindowManager.h"								// Used to reference the abstract base class


// This is our implementation of the abstract base class WindowManager, which uses the 
// GLFW cross-platform library to create a window, handle input and create the OpenGL
// context.  You can go online for to the first OpenGL tutorials page for the Github 
// "Gist" which has a Win32 implementation, for those who would rather work with that API.
class GLFWManager : public WindowManager
{
public:

	// Create an empty constructor and have the deconstructor release our memory
	GLFWManager()	{ }
	~GLFWManager()	{ Destroy(); }

	// This initialized the window and creates the OpenGL context
	virtual int Initialize(int width, int height, std::string strTitle, bool bFullScreen = false) override;

	// This swaps the backbuffer to the front and should be called every frame
	virtual void SwapTheBuffers() override;

	// This processes any needed input for the application, like the Escape key
	virtual bool ProcessInput(bool continueGame) override;

	// This destroys the window and OpenGL context
	virtual void Destroy();

protected:

	// This is the GLFW window object that manages the window and input
	GLFWwindow* Window;
};