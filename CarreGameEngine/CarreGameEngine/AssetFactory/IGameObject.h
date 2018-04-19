		/*
		*
		* @class IGameObject
		* @brief 
		*
		* @author
		* @version 1.0
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
		*
		* @return null
		*/
	IGameObject() { }
		/**
		* @brief IGameObject destructor
		*
		* @return null
		*/
	~IGameObject() { }

		/**
		* @brief Loads a file from the given path string
		* @param std::string filePath
		*
		* @return void
		*/
	virtual void LoadFromFilePath(std::string filePath) = 0;

		/**
		* @brief Load function for objects eg. NPC
		*
		* @return void
		*/
	virtual const void Load() = 0;

		/**
		* @brief Unload function for objects
		*
		* @return void
		*/
	virtual const void Unload() = 0;

		/**
		* @brief Returns the file path
		*
		* @return std::string
		*/

	virtual const std::string& GetFilePath() const = 0;
		/**
		* @brief Sets the file path
		* @param std::string& filePath
		*
		* @return void
		*/
	virtual void SetFilePath(const std::string& filePath) = 0;
};