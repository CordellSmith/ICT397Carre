/*
*
* @class GameAssetFactory
* @brief Responsible for creating the assets used within the game world. 	
*
* @date 29/04/2018
*/

#pragma once

#include <iostream>
#include <vector>

#include "IGameObject.h"

class GameAssetFactory
{
public:
	/**
	* @brief Default constructor
	*/
	GameAssetFactory() { }
	/**
	* @brief Destructor
	*/
	~GameAssetFactory() { }

	/**
	* @brief Creates and returns an object depending on type and obj file passed
	* @param type - defines what type of object it is(Terrain, Object, NPC)
	* @param filePath - path to the file containing the asset OBJ
	*/
	IGameObject* CreateAsset(OBJ_TYPE type, std::string filePath);

protected:
};