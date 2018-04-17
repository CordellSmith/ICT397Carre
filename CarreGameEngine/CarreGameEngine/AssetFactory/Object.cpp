#include "Object.h"

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
