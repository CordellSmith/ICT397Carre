#include "Terrain.h"

Terrain::Terrain(std::string filePath) : m_filePath(filePath)
{
	m_objType = OBJ_TERRAIN;
	LoadFromFilePath(filePath);
}

void Terrain::LoadFromFilePath(std::string filePath)
{
	//m_model->LoadModel(filePath);
}

const void Terrain::Load()
{
	return void();
}

const void Terrain::Unload()
{
	return void();
}

const void Terrain::Render(std::string vertShader, std::string fragShader)
{
	// Needs to be implemented
	return void();
}

const std::string & Terrain::GetFilePath() const
{
	return m_filePath;
}

void Terrain::SetFilePath(const std::string & filePath)
{
	m_filePath = filePath;
}
