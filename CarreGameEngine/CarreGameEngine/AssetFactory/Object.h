#pragma once

#include "IGameObject.h"
#include "..\headers\Model.h"

class Object : public IGameObject
{
public:
	Object() { }
	~Object() { }

	Object(std::string filePath);

	virtual void LoadFromFilePath(std::string filePath);

	virtual const void Load();
	virtual const void Unload();

	virtual const std::string & GetFilePath() const;
	virtual void SetFilePath(const std::string & filePath);

protected:
	std::string m_filePath;
	Model* m_model;
};