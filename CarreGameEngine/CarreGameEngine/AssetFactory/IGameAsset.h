#pragma once

#include <iostream>

#include "..\headers\Camera.h"
#include "..\AssetFactory\Model.h"

	/**
	* @brief Enumerator for asset types
	*
	* This enumerator is used to assign different types of assets to
	* a corresponding value. Used in creation of assets.
	*
	* @note Currently not entirely implemented.
	*
	* @author Cordell Smith
	* @version 01
	* @date 29/03/2018 CS
	*
	* @version 02
	* @date 31/05/2018
	*/
typedef enum
{
	ASS_NULL = 0,
	ASS_OBJECT = 1,
	ASS_NPC = 2,
	ASS_PLAYER = 3
}ASS_TYPE;

	/**
	* @class IGameAsset
	* @brief Interface class for game assets
	*
	* This class is an interface class for other assets to inherit from.
	* Contains various helper functions that are to be overridden in the child
	* classes.
	*
	* @author Cordell Smith
	* @version 01
	* @date 29/03/2018 CS
	*
	*/
class IGameAsset
{
public:
		/**
		* @brief Default constructor
		*
		* Empty default constructor.
		*
		* @return null
		*/
	IGameAsset() { }

		/**
		* @brief Default constructor
		*
		* Empty destructor.
		*
		* @return null
		*/
	~IGameAsset() { }

		/**
		* @brief Load object from file path
		*
		* Virtual function taking a file path as a string parameter to load a file.
		*
		* @param std::string filePath
		* @return void
		*/
	virtual void LoadFromFilePath(std::string filePath) = 0;

	virtual const void Destroy() = 0;

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

	virtual const ASS_TYPE GetOBJType() const { return m_assetType; }

	virtual const std::string GetAssetName() const { return m_assetName; }

		/**
		* @brief Sets the camera object
		*
		* Sets the camera object to the world camera object to retrieve the view and projection
		* matrices.
		*
		* @param Camera* camera
		* @return void
		*/
	virtual void SetCamera(Camera* camera) = 0;

	virtual void SetPosition(glm::vec3 position) = 0;

	virtual void SetScale(glm::vec3 scale) = 0;

	virtual void SetRotation(glm::vec3 rotation) = 0;

	virtual const glm::vec3 GetPosition() = 0;

	virtual const glm::vec3 GetRotation() = 0;

	virtual Model* GetModel() = 0;

	virtual void AddTexutre(GLuint textureId, std::string textureFilePath) = 0;

	virtual void SetAI(ComputerAI* compAI) = 0;
	virtual ComputerAI* GetAI() = 0;

protected:
	ASS_TYPE m_assetType;
	std::string m_assetName;
};