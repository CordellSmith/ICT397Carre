#pragma once

#include "IGameObject.h"
#include "..\headers\Model.h"

	/*
	* @class Terrain
	* @brief Terrain object
	*
	* This class is used to store all the data about an Terrain. It inherits from
	* the base class IGameObject and allows for initialisation and drawing of
	* the Terrain.
	*
	* @note Currently not entirely implemented.
	*
	* @author Cordell Smith
	* @version 01
	* @date 29/03/2018 CS
	*
	*/
class Terrain : public IGameObject
{
public:
		/**
		* @brief Default constructor
		*
		* Empty default constructor.
		*
		* @return null
		*/
	Terrain() { }

		/**
		* @brief Destructor
		*
		* Empty destructor.
		*
		* @return null
		*/
	~Terrain() { }

		/**
		* @brief Parameter constructor
		*
		* Takes the parameter string file path and assigns it to the member
		* variable m_filePath. It then parses this into the LoadFromFilePath()
		* function.
		*
		* @param std::string filePath
		* @return null
		*/
	Terrain(std::string filePath);

		/**
		* @brief Loads a Terrain from file
		*
		* Takes the parameter string file path and loads the Terrain data into the
		* corresponding model member variable.
		*
		* @param std::string filePath
		* @return void
		*/
	virtual void LoadFromFilePath(std::string filePath);

		/**
		* @brief Loads
		*
		* Loads the Terrain.
		*
		* @return void
		*/
	virtual const void Load();

		/**
		* @brief Unloads
		*
		* Unloads the Terrain.
		*
		* @return void
		*/
	virtual const void Unload();

		/**
		* @brief Renders
		*
		* Renders the Terrain.
		*
		* @return void
		*/
	virtual const void Render();

		/**
		* @brief Gets the file path
		*
		* Returns the file path containing the object information.
		*
		* @return std::string
		*/
	virtual const std::string & GetFilePath() const;

		/**
		* @brief Sets the file path
		*
		* Sets the file path containing the Terrain information.
		*
		* @param std::string& filePath
		* @return void
		*/
	virtual void SetFilePath(const std::string & filePath);
	
protected:
	/// Stores the file path containing the data
	std::string m_filePath;
	/// Model object
	Model* m_model;
};