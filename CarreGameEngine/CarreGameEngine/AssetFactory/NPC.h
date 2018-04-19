#pragma once

		/**
		* @class NPC
		* @brief NPC Class to handle loading and unloading of the NPC models 
		*
		* @author
		* @version 01
		* @date 29/03/2018 CS
		*
		*/

#include "IGameObject.h"
#include "..\headers\Model.h"

class NPC : public IGameObject
{
public:
		/**
		* @brief Default constructor
		*
		* @return null
		*/
	NPC() { }
		/**
		* @brief Destructor
		*
		* This function is required for the inheriting class to have its destructor called.
		*
		* @return null
		*/
	~NPC() { }

		/**
		* @brief NPC Constructor
		*
		* Called when a file path is passed to the function
		*
		* @param std::string filePath
		*
		* @return NPC
		*/
	NPC(std::string filePath);

		/**
		* @brief Load from file
		*
		* Loads an NPC from the supplied file path
		*
		* @param std::string filePath
		*
		* @return void
		*/
	virtual void LoadFromFilePath(std::string filePath);

		/**
		* @brief Load NPC function
		*
		* @return void
		*/
	virtual const void Load();
		/**
		* @brief Unload NPC function
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