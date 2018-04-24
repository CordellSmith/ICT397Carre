#pragma once

#include "IGameObject.h"
#include "NewModel.h"

	/*
	* @class Object
	* @brief Object (static/dynamic)
	*
	* This class is used to store all the data about an Object. It inherits from
	* the base class IGameObject and allows for initialisation and drawing of
	* the object.
	*
	* @note Currently not entirely implemented.
	*
	* @author Cordell Smith
	* @version 01
	* @date 29/03/2018 CS
	*
	*/
class Object : public IGameObject
{
public:
		/**
		* @brief Default constructor
		*
		* Empty default constructor.
		*
		* @return null
		*/
	Object() { }

		/**
		* @brief Destructor
		*
		* Empty destructor.
		*
		* @return null
		*/
	~Object() { }

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
	Object(std::string filePath);

		/**
		* @brief Loads an object from file
		*
		* Takes the parameter string file path and loads the NPC data into the
		* corresponding model member variable.
		*
		* @param std::string filePath
		* @return void
		*/
	virtual void LoadFromFilePath(std::string filePath);

		/**
		* @brief Loads
		*
		* Loads the object.
		*
		* @return void
		*/
	virtual const void Load();

		/**
		* @brief Unloads
		*
		* Unloads the object.
		*
		* @return void
		*/
	virtual const void Unload();

		/**
		* @brief Renders
		*
		* Renders the Object.
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
		* Sets the file path containing the object information.
		*
		* @param std::string& filePath
		* @return void
		*/
	virtual void SetFilePath(const std::string & filePath);

protected:
	/// Stores the file path containing the data
	std::string m_filePath;
	/// Model object
	NewModel* m_model;
};