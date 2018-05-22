#pragma once

#include <iostream>

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

	void MoveForward(float speed);
	void MoveBackward(float speed);
	void TurnClock(float speed);
	void TurnAntiClock(float speed);
	
	const std::string & GetFilePath() const { return m_filePath; }
	void SetFilePath(const std::string& filePath) { m_filePath = filePath; }

	void SetCamera(Camera * camera) { m_playerModel->SetCamera(camera); }

	glm::vec3 GetPosition() const { return m_playerModel->GetPosition(); }
	void SetPosition(glm::vec3 position) { m_playerModel->SetPosition(position); }

	glm::vec3 GetRotation() const { return m_playerModel->GetRotation(); }
	void SetRotation(glm::vec3 rotation) { m_playerModel->SetRotation(rotation); }

	void SetScale(glm::vec3 scale) { m_playerModel->SetScale(scale); }

	float GetMoveSpeed() { return m_moveSpeed; }

protected:
	std::string m_filePath;
	std::string m_name;
	
	Model* m_playerModel;

	float m_currentMoveSpeed;
	float m_currentTurnSpeed;
	float m_moveSpeed;
	float m_turnSpeed;
};