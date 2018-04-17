#include "../headers/ScriptManager.h"

int loadScript()
{
	//Create and start our environment
	lua_State *Environment = lua_open();

	//Check for errors
	if(Environment == NULL)
	{
		printf("Error loading script");
		return(0);
	}

	//Load libs
	luaL_openlibs(Environment);

	//Run a test lua file
	printf("Running script..");
	luaL_dofile(Environment, "res/scripts/test.lua");

	//Call a function from a script
	luaL_loadfile(Environment, "res/scripts/test2.lua");
	lua_call(Environment, 0, 0); //0 Arguments and 0 returns

	//Close our environment
	lua_close(Environment);
	return(0);
}