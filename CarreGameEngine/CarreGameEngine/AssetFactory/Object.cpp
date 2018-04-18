#include "Object.h"

Object::Object(std::string filePath) : m_filePath(filePath)
{
	LoadFromFilePath(filePath);
}

void Object::LoadFromFilePath(std::string filePath)
{
	//m_model->LoadModel(filePath);
}

const void Object::Load()
{
	return void();
}

const void Object::Unload()
{
	return void();
}

const std::string & Object::GetFilePath() const
{
	return m_filePath;
}

void Object::SetFilePath(const std::string & filePath)
{
	m_filePath = filePath;
}
