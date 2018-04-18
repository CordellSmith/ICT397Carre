#include "Terrain.h"

Terrain::Terrain(std::string filePath) : m_filePath(filePath)
{
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

const std::string & Terrain::GetFilePath() const
{
	return m_filePath;
}

void Terrain::SetFilePath(const std::string & filePath)
{
	m_filePath = filePath;
}
