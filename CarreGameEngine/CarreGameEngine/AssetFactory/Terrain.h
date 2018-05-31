#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "Model.h"
#include "..\headers\TextureManager.h"

	/**
	* @class
	* @brief
	*
	* @author Cordell Smith
	* @version 01
	* @date 31/05/2018
	*
	*/
class Terrain
{
public:
	Terrain();
	~Terrain() { }
	Terrain(float scaleX, float scaleY, float scaleZ);

	virtual void GenerateTerrain(GLuint textureId, std::string textureFilePath) = 0;

	void SetCamera(Camera* camera) { m_terrainModel->SetCamera(camera); }
	void SetPosition(glm::vec3 position) { m_terrainModel->SetPosition(position); }
	void SetScale(glm::vec3 scale) { m_terrainModel->SetScale(scale); }

	Model* GetModel() { return m_terrainModel; }
	
protected:
	Model* m_terrainModel;
	unsigned char* m_terrainData;
	float m_scaleX, m_scaleY, m_scaleZ;
	int m_heightfieldSize;

	GLuint m_textureId;
};