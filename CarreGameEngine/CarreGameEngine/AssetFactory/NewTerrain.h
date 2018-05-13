#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "NewModel.h"
#include "..\headers\TextureManager.h"

class NewTerrain
{
public:
	NewTerrain();
	~NewTerrain() { }
	NewTerrain(float scaleX, float scaleY, float scaleZ);

	virtual void GenerateTerrain() = 0;
	virtual void AddShader(std::string vertShader, std::string fragShader) = 0;
	virtual void Render() = 0;

	void SetCamera(Camera* camera) { m_terrainModel->SetCamera(camera); }
	void SetPosition(glm::vec3 position) { m_terrainModel->SetPosition(position); }
	void SetScale(glm::vec3 scale) { m_terrainModel->SetScale(scale); }
	
protected:
	NewModel* m_terrainModel;
	unsigned char* m_terrainData;
	float m_scaleX, m_scaleY, m_scaleZ;
	int m_heightfieldSize;

	GLuint m_textureId;
};