--TerrainsInit.lua
--Author: Jack Matters
--Date: 23/05/2018
--Brief: Load in all heightmaps data
--Note: Make filePath relative to CarreGameEngine.vcxproj
--Note: Use this file to load all heightmap data
--Note: "player" MUST BE present and MUST BE spelt correctly
--Note: Needs improvements
AllHeightmaps=
{
	heightmap=
	{
		{filePath = "res/terrain/newcity.raw", texFilePath = "res/terrain/grass.jpg", fileSize = 128, scaleX = 15.0, scaleY = 16.0, scaleZ = 17.0, posX = 0.0, posY = 150.0, posZ = 200.0},
		{filePath = "res/terrain/buildingheightmap.raw", texFilePath = "res/terrain/buildingtexture.jpg", fileSize = 25, scaleX = 26.0, scaleY = 27.0, scaleZ = 15.0, posX = 100.0, posY = 150.0, posZ = 200.0},
	},
}
