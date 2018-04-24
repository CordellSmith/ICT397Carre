#include "Object.h"

Object::Object(std::string filePath) : m_filePath(filePath)
{
	m_objType = OBJ_OBJECT;
	m_model = new NewModel();
	LoadFromFilePath(filePath);
}

void Object::LoadFromFilePath(std::string filePath)
{
	m_model->LoadModel(filePath);
	m_model->SetCamera(m_camera);
}

const void Object::Load()
{
	return void();
}

const void Object::Unload()
{
	return void();
}

const void Object::Render(std::string vertShader, std::string fragShader)
{
	m_model->Draw(vertShader, fragShader);
}

const std::string & Object::GetFilePath() const
{
	return m_filePath;
}

void Object::SetFilePath(const std::string & filePath)
{
	m_filePath = filePath;
}
