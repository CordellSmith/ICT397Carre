		/**
		* @class Object
		* @brief 
		* Main Object class for loading and unloading objects from file
		*
		* @author
		* @version 01
		* @date 29/03/2018 CS
		*
		*/
#pragma once

#include "IGameObject.h"
#include "..\headers\Model.h"

class Object : public IGameObject
{
public:
		/**
		* @brief Default constructor
		*
		* @return null
		*/
	Object() { }
		/**
		* @brief Destructor
		*
		* @return null
		*/
	~Object() { }

		/**
		* @brief Constructor taking in file path
		* @ param std::string filePath
		*
		* @return Object
		*/
	Object(std::string filePath);

		/**
		* @brief Load from file function
		* @param std::string filePath
		*
		* @return void
		*/
	virtual void LoadFromFilePath(std::string filePath);

		/**
		* @brief Load Object
		*
		* @return void
		*/
	virtual const void Load();
		/**
		* @brief Unload oject
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