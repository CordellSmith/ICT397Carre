#pragma once

#include "IGameObject.h"

class Terrain : public IGameObject
{
public:
	Terrain() { }
	~Terrain() { }

	virtual const void Load();
	virtual const void Unload();

	virtual const std::string & GetFilePath() const;
	virtual void SetFilePath(const std::string & filePath);

protected:
	std::string m_filePath;

};