#pragma once

#include <string>										
#include <fstream>															
#include "GLFW\glfw3.h"									
#include "WindowManager.h"								


/*
* @class GLFWManager
* @brief This is our implementation of the abstract base class WindowManager
*
* This class uses the GLFW cross-platform library to create a window, handle input and create the OpenGL context
* by inheriting from the interface class WindowManager.
*
* @author Cordell Smith
* @date 29/04/2018
*/
class GLFWManager : public WindowManager
{
public:

	/**
	* @brief Create an empty constructor
	*/
	GLFWManager()	{ }
	/**
	* @brief Deconstructor releases our memory
	*/
	~GLFWManager()	{ Destroy(); }

	/**
	* @brief This initialized the window and creates the OpenGL context
	*/
	virtual int Initialize(int width, int height, std::string strTitle, bool bFullScreen = false) override;

	/**
	* @brief This swaps the backbuffer to the front and should be called every frame
	*/
	virtual void SwapTheBuffers() override;
	/**
	* @brief This processes any needed input for the application, like the Escape key
	*/
	virtual bool ProcessInput(bool continueGame) override;
	/**
	* @brief This destroys the window and OpenGL context
	*/
	virtual void Destroy();

protected:
	/**
	* @brief This is the GLFW window object that manages the window and input
	*/
	GLFWwindow* Window;
};