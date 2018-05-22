#include "Player.h"

Player::Player(std::string playerName)
{
	m_name = playerName;
	m_playerModel = new Model();

	m_currentMoveSpeed = 0;
	m_currentTurnSpeed = 0;
	m_moveSpeed = 10;
	m_turnSpeed = 0.1;
}

void Player::LoadFromFilePath(std::string filePath)
{
	m_playerModel->LoadModel(filePath);
}

const void Player::Prepare(std::string vertShader, std::string fragShader)
{
	m_playerModel->Prepare(vertShader, fragShader);
}

const void Player::Render()
{
	m_playerModel->Draw();
}

const void Player::Destroy()
{
	return void();
}

void Player::MoveForward(float speed)
{
	m_currentMoveSpeed = m_moveSpeed * speed;

	float dx = (float)(m_currentMoveSpeed * glm::sin(m_playerModel->GetRotation().y));
	float dz = (float)(m_currentMoveSpeed * glm::cos(m_playerModel->GetRotation().y));

	m_playerModel->SetPosition(glm::vec3(m_playerModel->GetPosition().x + dx, 0.0, m_playerModel->GetPosition().z + dz));
}

void Player::MoveBackward(float speed)
{
	m_currentMoveSpeed = m_moveSpeed * speed;;

	float dx = (float)(m_currentMoveSpeed * glm::sin(glm::radians(m_playerModel->GetRotation().y)));
	float dz = (float)(m_currentMoveSpeed * glm::cos(glm::radians(m_playerModel->GetRotation().y)));

	m_playerModel->SetPosition(glm::vec3(m_playerModel->GetPosition().x - dx, 0.0, m_playerModel->GetPosition().z - dz));
}

void Player::TurnClock(float speed)
{
	m_currentTurnSpeed = m_turnSpeed * speed;
	m_playerModel->SetRotation(glm::vec3(0.0, m_playerModel->GetRotation().y - m_currentTurnSpeed, 0.0));
}

void Player::TurnAntiClock(float speed)
{
	m_currentTurnSpeed = m_turnSpeed * speed;
	m_playerModel->SetRotation(glm::vec3(0.0, m_playerModel->GetRotation().y + m_currentTurnSpeed, 0.0));
}
