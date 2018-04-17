#pragma once

#include "IGameObject.h"

class NPC : public IGameObject
{
public:
	NPC() { }
	~NPC() { }

	virtual const void Load();
	virtual const void Unload();

	virtual const std::string & GetFilePath() const;
	virtual void SetFilePath(const std::string & filePath);

protected:
	std::string m_filePath;

};