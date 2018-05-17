/*
* Implementation of TextureManager.h file
* Author - Jack Matters
*/

// Includes
#include "../headers/ScriptManager.h"
#include <iostream>

// Default constructor
ScriptManager::ScriptManager(){}

// De-constructor
ScriptManager::~ScriptManager(){}

void ScriptManager::CloseLuaState()
{
	//lua_close(Environment);
}

// Load all scripts
void ScriptManager::LoadAllLuaScripts()
{
	// Read in file containing all lua script file names
	// For each line read, call appropriate function
	// Do this later

	// Load all camera and window init variables
	//LoadCamInitLua();
}

// Load all camera and window init variables
bool ScriptManager::LoadWindowInitLua(int &width, int &height, std::string &name, bool &fullScreen)
{
	// Create lua state
	lua_State* Environment = lua_open();
	if (Environment == NULL)
	{
		// Show error and exit program
		std::cout << "Error Initializing lua.." << std::endl;
		getchar();
		exit(0);
	}

	// Load standard lua library functions
	luaL_openlibs(Environment);

	// Load and run script
	if (luaL_dofile(Environment, "res/scripts/WindowInit.lua"))
	{
		std::cout << "Error opening file.." << std::endl;
		getchar();
		return false;
	}

	// Read from script
	lua_settop(Environment, 0);
	lua_getglobal(Environment, "screenWidth");
	lua_getglobal(Environment, "screenHeight");
	lua_getglobal(Environment, "screenTitle");
	lua_getglobal(Environment, "fullScreen");

	// Set values
	width = (int)lua_tonumber(Environment, 1);
	height = (int)lua_tonumber(Environment, 2);
	name = lua_tostring(Environment, 3);
	fullScreen = lua_toboolean(Environment, 4);

	// Close environment
	lua_close(Environment);

	// Return true for successful loading and reading
	return true;
}

// Load all camera and viewing matrix variables
bool ScriptManager::LoadCamInitLua(glm::vec3 &camPos, float &yaw, float &pitch, float &fov, float &near, float &far)
{
	// Create lua state
	lua_State* Environment = lua_open();
	if (Environment == NULL)
	{
		// Show error and exit program
		std::cout << "Error Initializing lua.." << std::endl;
		getchar();
		exit(0);
	}

	// Load standard lua library functions
	luaL_openlibs(Environment);

	// Load and run script
	if (luaL_dofile(Environment, "res/scripts/CameraInit.lua"))
	{
		std::cout << "Error opening file.." << std::endl;
		getchar();
		return false;
	}

	// Read from script
	lua_settop(Environment, 0);
	lua_getglobal(Environment, "camPosX");
	lua_getglobal(Environment, "camPosY");
	lua_getglobal(Environment, "camPosZ");
	lua_getglobal(Environment, "yaw");
	lua_getglobal(Environment, "pitch");
	lua_getglobal(Environment, "camFOV");
	lua_getglobal(Environment, "camNearPlane");
	lua_getglobal(Environment, "camFarPlan");

	// Set values
	camPos.x = (int)lua_tonumber(Environment, 1);
	camPos.y = (float)lua_tonumber(Environment, 2);
	camPos.z = (float)lua_tonumber(Environment, 3);
	yaw = (float)lua_tonumber(Environment, 4);
	pitch = (float)lua_tonumber(Environment, 5);
	fov = (float)lua_tonumber(Environment, 6);
	near = (float)lua_tonumber(Environment, 7);
	far = (float)lua_tonumber(Environment, 8);

	// Close environment
	lua_close(Environment);

	// Return true for successful loading and reading
	return true;
}