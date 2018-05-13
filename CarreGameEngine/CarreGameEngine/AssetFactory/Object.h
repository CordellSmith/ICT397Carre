#pragma once

#include "IGameAsset.h"
#include "NewModel.h"

	/*
	* @class Object
	* @brief Object asset (static/dynamic)
	*
	* This class is used to store all the data about an Object. It inherits from
	* the base class IGameAsset and allows for initialisation and drawing of
	* the object.
	*
	* @note Currently not entirely implemented.
	*
	* @author Cordell Smith
	* @version 01
	* @date 29/03/2018 CS
	*
	*/
class Object : public IGameAsset
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
	~Object() { Destroy(); }

		/**
		* @brief Parameter constructor
		*
		* Takes the parameter string asset name and creates a new
		* Object with that name identifier. Initialises the model of the
		* obejct as well.
		*
		* @param std::string assetName
		* @return null
		*/
	Object(std::string assetName);

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

	virtual const void Prepare(std::string vertShader, std::string fragShader);

		/**
		* @brief Renders
		*
		* Renders the Object.
		*
		* @return void
		*/
	virtual const void Render();

	virtual const void Destroy();

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
	virtual void SetFilePath(const std::string& filePath);

	virtual void SetCamera(Camera* camera) { m_model->SetCamera(camera); }

	virtual void SetAssetPosition(glm::vec3 position) { m_model->SetPosition(position); }
	
	virtual void SetAssetScale(glm::vec3 scale) { m_model->SetScale(scale); }

	NewModel* GetModel() { return m_model; }
	void SetModel(NewModel* model) { m_model = model; }

protected:
	/// Stores the file path containing the data
	std::string m_filePath;
	/// Model object
	NewModel* m_model;

	Camera* m_camera;
};