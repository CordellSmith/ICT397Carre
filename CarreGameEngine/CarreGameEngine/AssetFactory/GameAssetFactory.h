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
		*
		* @return null
		*/
	GameAssetFactory() { }
		/**
		* @brief Destructor
		*
		* @return null
		*/
	~GameAssetFactory() { }

		/**
		* @brief Creates and returns an object depending on type and obj file passed
		*
		* @param OBJ_TYPE type 
		* @param std::string filePath
		*
		* @return IGameObject*
		*/
	IGameObject* CreateAsset(OBJ_TYPE type, std::string filePath);

protected:
};