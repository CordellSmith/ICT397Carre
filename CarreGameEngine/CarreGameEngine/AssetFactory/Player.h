#pragma once

#include<iostream>

#include "IGameAsset.h"
#include "Model.h"

class Player 
{
public:
	Player(std::string assetName);
	~Player() { }

	void LoadFromFilePath(std::string filePath);

	const void Prepare(std::string vertShader, std::string fragShader);

	const void Render();

	const void Destroy();

	const std::string & GetFilePath() const { return m_filePath; }

	void SetFilePath(const std::string& filePath) { m_filePath = filePath; }

	void SetCamera(Camera * camera) { m_playerModel->SetCamera(camera); }

	void SetPosition(glm::vec3 position) { m_playerModel->SetPosition(position); }

	void SetScale(glm::vec3 scale) { m_playerModel->SetScale(scale); }

	void SetRotation(glm::vec3 rotation) { m_playerModel->SetRotation(rotation); }

	glm::vec3 GetPosition() const { return m_playerModel->GetPosition(); }

	glm::vec3 GetRotation() const { return m_playerModel->GetRotation(); }

protected:
	std::string m_filePath;
	std::string m_name;
	
	Model* m_playerModel;

	Camera* m_camera;
};