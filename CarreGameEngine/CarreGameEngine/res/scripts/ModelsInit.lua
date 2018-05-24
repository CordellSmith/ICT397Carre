--ModelInit.lua
--Author: Jack Matters
--Date: 17/05/2018
--Brief: Load in all models and their positions
--Note: Make filePath relative to CarreGameEngine.vcxproj
--Note: Use this file to load model .obj, scale, and position only
--Note: "player" MUST BE present and MUST BE spelt correctly
--Note: Needs improvements
AllModels=
{
	cube=
	{
		{filePath = "res/objects/cube.obj", scaleX = 15.0, scaleY = 15.0, scaleZ = 15.0, posX = 0.0, posY = 150.0, posZ = 200.0},
		{filePath = "res/objects/cube.obj", scaleX = 15.0, scaleY = 15.0, scaleZ = 15.0, posX = 100.0, posY = 150.0, posZ = 100.0},
		{filePath = "res/objects/cube.obj", scaleX = 15.0, scaleY = 15.0, scaleZ = 15.0, posX = 200.0, posY = 150.0, posZ = 200.0},
		{filePath = "res/objects/cube.obj", scaleX = 15.0, scaleY = 15.0, scaleZ = 15.0, posX = 300.0, posY = 150.0, posZ = 100.0},
		{filePath = "res/objects/cube.obj", scaleX = 15.0, scaleY = 15.0, scaleZ = 15.0, posX = 400.0, posY = 150.0, posZ = 200.0},
		{filePath = "res/objects/cube.obj", scaleX = 15.0, scaleY = 15.0, scaleZ = 15.0, posX = 500.0, posY = 150.0, posZ = 100.0},
	},
	player=
	{
		{filePath = "res/objects/taxi/taxi.obj", scaleX = 50.0, scaleY = 50.0, scaleZ = 50.0, posX = 900.0, posY = 150.0, posZ = 0.0},
	},
	trafficLight=
	{
		{filePath = "res/objects/trafficlight/trafficlight.obj", scaleX = 15.0, scaleY = 15.0, scaleZ = 15.0, posX = 300.0, posY = 150.0, posZ = 300.0},
		{filePath = "res/objects/trafficlight/trafficlight.obj", scaleX = 15.0, scaleY = 15.0, scaleZ = 15.0, posX = 400.0, posY = 150.0, posZ = 400.0},
		{filePath = "res/objects/trafficlight/trafficlight.obj", scaleX = 15.0, scaleY = 15.0, scaleZ = 15.0, posX = 500.0, posY = 150.0, posZ = 500.0},
	},
}
