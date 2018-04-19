/*
*
* @class IGameObject
* @brief 
*
* @date 29/04/2018
*/

#pragma once

#include <iostream>

/**
* @brief Enum for the various object types
*/
typedef enum
{
	OBJ_TERRAIN = 0,
	OBJ_OBJECT = 1,
	OBJ_NPC = 2
}OBJ_TYPE;

class IGameObject
{
public:
	/**
	* @brief IGameObject default constructor
	*/
	IGameObject() { }
	/**
	* @brief IGameObject destructor
	*/
	~IGameObject() { }

	/**
	* @brief Loads a file from the given path string
	* @param filePath - Takes in a path string for the file
	*/
	virtual void LoadFromFilePath(std::string filePath) = 0;

	/**
	* @brief Load function for objects eg. NPC
	*/
	virtual const void Load() = 0;
	/**
	* @brief Unload function for objects
	*/
	virtual const void Unload() = 0;

	/**
	* @brief Returns the file path
	*/
	virtual const std::string& GetFilePath() const = 0;
	/**
	* @brief Sets the file path
	*/
	virtual void SetFilePath(const std::string& filePath) = 0;
};