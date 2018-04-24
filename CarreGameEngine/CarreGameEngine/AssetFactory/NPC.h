#pragma once

#include "IGameObject.h"
#include "..\headers\Model.h"

	/*
	* @class NPC
	* @brief NPC object
	*
	* This class is used to store all the data about an NPC. It inherits from
	* the base class IGameObject and allows for initialisation and drawing of
	* the NPC object.
	*
	* @note Currently not entirely implemented.
	*
	* @author Cordell Smith
	* @version 01
	* @date 29/03/2018 CS
	*
	*/
class NPC : public IGameObject
{
public:
		/**
		* @brief Default constructor
		*
		* Empty default constructor.
		*
		* @return null
		*/
	NPC() { }

		/**
		* @brief Destructor
		*
		* Empty destructor.
		*
		* @return null
		*/
	~NPC() { }

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
	NPC(std::string filePath);

		/**
		* @brief Loads an NPC from file
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
		* Loads the NPC.
		*
		* @return void
		*/
	virtual const void Load();

		/**
		* @brief Unloads
		*
		* Unloads the NPC.
		*
		* @return void
		*/
	virtual const void Unload();

		/**
		* @brief Renders
		*
		* Renders the NPC.
		*
		* @return void
		*/
	virtual const void Render(std::string vertShader, std::string fragShader);

		/**
		* @brief Gets the file path
		*
		* Returns the file path containing the NPC information.
		*
		* @return std::string
		*/
	virtual const std::string & GetFilePath() const;

		/**
		* @brief Sets the file path
		*
		* Sets the file path containing the NPC information.
		*
		* @param std::string& filePath
		* @return void
		*/
	virtual void SetFilePath(const std::string& filePath);

	virtual void SetObjectPosition(glm::vec3 position) { m_model->SetPosition(position); }

protected:
	/// Stores the file path containing the data
	std::string m_filePath;
	/// Model object
	Model* m_model;
};