#include "Object.h"

Object::Object(std::string assetName)
{
	m_assetType = ASS_OBJECT;
	m_assetName = assetName;
	m_model = new NewModel();
}

void Object::LoadFromFilePath(std::string filePath)
{
	m_model->LoadModel(filePath);
	m_model->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
}

const void Object::Load()
{
	return void();
}

const void Object::Unload()
{
	return void();
}

const void Object::Prepare(std::string vertShader, std::string fragShader)
{
	m_model->Prepare(vertShader, fragShader);
}

const void Object::Render()
{
	m_model->Draw();
}

const void Object::Destroy()
{
	m_model->~NewModel();
}

const std::string & Object::GetFilePath() const
{
	return m_filePath;
}

void Object::SetFilePath(const std::string & filePath)
{
	m_filePath = filePath;
}
