--ModelInit.lua
--Author: Jack Matters
--Date: 17/05/2018
--Brief: Load in all models and their positions
--Note: Make filePath relative to CarreGameEngine.vcxproj
--Note: Use this file to load model .obj, scale, and position only
--Note: posY not required, as that is determined by terrain GetHeight() function
--Note: "player" MUST BE present and MUST BE spelt correctly
--Note: Needs improvements
AllModels=
{
	monsterOne=
	{
		{filePath = "res/objects/monster/monster1.obj", scaleX = 41.0, scaleY = 42.0, scaleZ = 43.0, posX = 300.0, posZ = 300.0},
		{filePath = "res/objects/monster/monster1.obj", scaleX = 51.0, scaleY = 52.0, scaleZ = 53.0, posX = 300.0, posZ = 300.0},
		{filePath = "res/objects/monster/monster1.obj", scaleX = 61.0, scaleY = 62.0, scaleZ = 63.0, posX = 600.0, posZ = 500.0},
	},
	player=
	{
		{filePath = "res/objects/taxi/taxi.obj", scaleX = 50.0, scaleY = 50.0, scaleZ = 50.0, posX = 700.0, posY = 150.0, posZ = 0.0},
	},
	monsterTwo=
	{
		{filePath = "res/objects/monster/monster1.obj", scaleX = 41.0, scaleY = 42.0, scaleZ = 43.0, posX = 300.0, posZ = 300.0},
		{filePath = "res/objects/monster/monster1.obj", scaleX = 51.0, scaleY = 52.0, scaleZ = 53.0, posX = 300.0, posZ = 300.0},
		{filePath = "res/objects/monster/monster1.obj", scaleX = 61.0, scaleY = 62.0, scaleZ = 63.0, posX = 600.0, posZ = 500.0},
	},
}
