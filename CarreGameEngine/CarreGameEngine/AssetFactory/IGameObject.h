#pragma once

#include <iostream>

	/**
	* @brief Enumerator for object types
	*
	* This enumerator is used to assign different types of objects to
	* a corresponding value. Used in creation of objects.
	*
	* @note Currently not entirely implemented.
	*
	* @author Cordell Smith
	* @version 01
	* @date 29/03/2018 CS
	*/
typedef enum
{
	OBJ_TERRAIN = 0,
	OBJ_OBJECT = 1,
	OBJ_NPC = 2
}OBJ_TYPE;


	/**
	* @class IGameObject
	* @brief Interface class for game objects
	*
	* This class is an interface class for other objects to inherit from.
	* Contains various helper functions that are to be overridden in the child
	* classes.
	*
	* @author Cordell Smith
	* @version 01
	* @date 29/03/2018 CS
	*
	*/
class IGameObject
{
public:
		/**
		* @brief Default constructor
		*
		* Empty default constructor.
		*
		* @return null
		*/
	IGameObject() { }

		/**
		* @brief Default constructor
		*
		* Empty destructor.
		*
		* @return null
		*/
	~IGameObject() { }

		/**
		* @brief Load object from file path
		*
		* Virtual function taking a file path as a string parameter to load a file.
		*
		* @param std::string filePath
		* @return void
		*/
	virtual void LoadFromFilePath(std::string filePath) = 0;

		/**
		* @brief Load function
		*
		* Virtual load function.
		*
		* @return void
		*/
	virtual const void Load() = 0;

		/**
		* @brief Unload function
		*
		* Virtual unload function.
		*
		* @return void
		*/
	virtual const void Unload() = 0;

		/**
		* @brief Render function
		*
		* Virtual render function to draw object to screen depending on type.
		*
		* @return void
		*/
	virtual const void Render() = 0;

		/**
		* @brief Gets file path
		*
		* Virtual function to return a file path.
		*
		* @return const std::string
		*/
	virtual const std::string& GetFilePath() const = 0;

		/**
		* @brief Sets a file path
		*
		* Virtual function to set a file path.
		*
		* @param const std::string& filePath
		* @return void
		*/
	virtual void SetFilePath(const std::string& filePath) = 0;

	virtual const OBJ_TYPE GetOBJType() const { return m_objType; }

protected:
	OBJ_TYPE m_objType;
};