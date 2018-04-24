#include "NPC.h"

NPC::NPC(std::string filePath) : m_filePath(filePath)
{
	m_objType = OBJ_NPC;
	LoadFromFilePath(filePath);
}

void NPC::LoadFromFilePath(std::string filePath)
{
	//m_model->LoadModel(filePath);
}

const void NPC::Load()
{
	return void();
}

const void NPC::Unload()
{
	return void();
}

const void NPC::Render()
{
	// Needs to be implemented
	return void();
}

const std::string & NPC::GetFilePath() const
{
	return m_filePath;
}

void NPC::SetFilePath(const std::string & filePath)
{
	m_filePath = filePath;
}
