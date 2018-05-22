#include "Object.h"

Object::Object(std::string assetName)
{
	m_assetType = ASS_OBJECT;
	m_assetName = assetName;
	m_model = new Model();
}

void Object::LoadFromFilePath(std::string filePath)
{
	m_model->LoadModel(filePath);
}

const void Object::Destroy()
{
	// Todo: destroy all items of Object class
}