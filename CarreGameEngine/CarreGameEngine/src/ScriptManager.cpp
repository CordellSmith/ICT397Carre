/*
* Implementation of TextureManager.h file
* Author - Jack Matters
*/

// Includes
#include "../headers/ScriptManager.h"
#include <iostream>

// Default constructor
ScriptManager::ScriptManager()
{
	//// Create lua state
	//Environment = lua_open();
	//if (Environment == NULL)
	//{
	//	// Show error and exit program
	//	std::cout << "Error Initializing lua.." << std::endl;
	//	getchar();
	//	exit(0);
	//}

	//// Load standard lua library functions
	//luaL_openlibs(Environment);
}

// De-constructor
ScriptManager::~ScriptManager()
{
	// Close lua state
	//lua_close(Environment);
}

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