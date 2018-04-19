		/*
		*
		* @class GLFW Manager
		* @brief This is our implementation of the abstract base class WindowManager, which uses the 
		*	GLFW cross-platform library to create a window, handle input and create the OpenGL
		*	context.  You can go online for to the first OpenGL tutorials page for the Github 
		*	"Gist" which has a Win32 implementation, for those who would rather work with that API.
		*
		* @author
		* @version 1.0
		* @date 29/04/2018
		*/

#pragma once

#include <string>										// Used for our STL string objects
#include <fstream>										// Used for fprintf and stderr									
#include "GLFW\glfw3.h"									// Used to reference the GLFW library
#include "WindowManager.h"								// Used to reference the abstract base class



class GLFWManager : public WindowManager
{
public:

		/**
		* @brief Create an empty constructor
		*
		* @return null
		*/
	GLFWManager()	{ }

		/**
		* @brief Deconstructor releases our memory
		*
		* @return null
		*/
	~GLFWManager()	{ Destroy(); }

		/**
		* @brief This initialized the window and creates the OpenGL context
		* @param int width
		* @param int height
		* @param std::string
		* @param bool bFullScreen
		*
		* @return int
		*/
	virtual int Initialize(int width, int height, std::string strTitle, bool bFullScreen = false) override;

		/**
		* @brief This swaps the backbuffer to the front and should be called every frame
		* 
		* @return void
		*/
	virtual void SwapTheBuffers() override;
		/**
		* @brief This processes any needed input for the application, like the Escape key
		* @param bool continueGame
		*
		* @return bool
		*/
	virtual bool ProcessInput(bool continueGame) override;
		/**
		* @brief This destroys the window and OpenGL context
		*
		* @return void
		*/
	virtual void Destroy();

protected:
	GLFWwindow* Window;
};