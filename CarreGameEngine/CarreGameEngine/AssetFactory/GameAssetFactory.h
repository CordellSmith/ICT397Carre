#pragma once

#include <iostream>
#include <vector>

#include "IGameObject.h"

	/*
	* @class GameAssetFactory
	* @brief Responsible for creating the assets used within the game world.
	*
	* This class is used to create any asset within the game control engine that may be
	* used in the game world. This includes objects (static and dynamic), terrain and NPCs etc.
	*
	* @note Currently not entirely implemented.
	*
	* @author Cordell Smith
	* @version 01
	* @date 29/03/2018 CS
	*
	*/
class GameAssetFactory
{
public:
		/**
		* @brief Default constructor
		*
		* Empty default constructor.
		*
		* @return null
		*/
	GameAssetFactory() { }

		/**
		* @brief Destructor
		*
		* Empty destructor.
		*
		* @return null
		*/
	~GameAssetFactory() { }

		/**
		* @brief Creates and returns an asset according to type
		*
		* Using the type parameter, checks to see what type of asset to return. The file path
		* parameter is used to locate the file to load.
		* 
		* @param OBJ_TYPE type
		* @param std::string filePath
		* @return IGameObject*
		*/
	IGameObject* CreateAsset(OBJ_TYPE type, std::string filePath);

protected:
};