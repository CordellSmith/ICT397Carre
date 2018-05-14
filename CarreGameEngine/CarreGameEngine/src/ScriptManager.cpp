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

}

// De-constructor
ScriptManager::~ScriptManager()
{

}

// Load all scripts
void ScriptManager::LoadAllLuaScripts()
{
	// Read in file containing all lua script file names
	// For each line read, call appropriate function
	// Do this later

	// Load all camera and window init variables
	LoadCamInitLua();
}

// Load all camera and window init variables
bool ScriptManager::LoadCamInitLua()
{
	// Create lua state
	lua_State* Environment = lua_open();
	if (Environment == NULL)
	{
		std::cout << "Error Initializing lua.." << std::endl;
		return false;
	}

	// Load standard lua library functions
	luaL_openlibs(Environment);

	// Load and run script
	if (luaL_dofile(Environment, "../Scripts/Script1.lua"))
	{
		std::cout << "Error opening file.." << std::endl;
		getchar();
		return false;
	}

	// Read from script
	lua_settop(Environment, 0);
	lua_getglobal(Environment, "levelName");
	lua_getglobal(Environment, "numSectors");
	lua_getglobal(Environment, "numRooms");


	// Do stuff here





	// Close lua state
	lua_close(Environment);

	// Return true for successful loading and reading
	return true;
}