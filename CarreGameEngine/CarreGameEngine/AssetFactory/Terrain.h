		/**
		* @class Terrain
		* @brief Terrain class to handle the loading 
		* and unloading of the terrain file
		*
		* @author
		* @version 01
		* @date 29/03/2018 CS
		*
		*/

#pragma once

#include "IGameObject.h"
#include "..\headers\Model.h"

class Terrain : public IGameObject
{
public:
		/**
		* @brief Default constructor
		* Create an empty constructor
		*
		* @return null
		*/
	Terrain() { }
		/**
		* @brief Destructor
		*
		* @return null
		*/
	~Terrain() { }

		/**
		* @brief Constructor taking in a file path
		* @param std::string filePath
		*
		* @return Terrain
		*/
	Terrain(std::string filePath);

		/**
		* @brief Load from file function
		* @param std::string filePath
		*
		* @return void
		*/
	virtual void LoadFromFilePath(std::string filePath);

		/**
		* @brief Load Terrain function
		*
		* @return void
		*/
	virtual const void Load();
		/**
		* @brief Unload terrain function
		*
		* @return void
		*/
	virtual const void Unload();

	virtual const std::string & GetFilePath() const;
	virtual void SetFilePath(const std::string & filePath);

protected:
	std::string m_filePath;
	Model* m_model;
};