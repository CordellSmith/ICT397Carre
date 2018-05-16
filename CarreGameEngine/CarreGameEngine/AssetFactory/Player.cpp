#include "Player.h"

Player::Player(std::string playerName)
{
	m_name = playerName;
	m_playerModel = new Model();
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