#pragma once

#include <iostream>

typedef enum
{
	OBJ_TERRAIN = 0,
	OBJ_OBJECT = 1,
	OBJ_NPC = 2
}OBJ_TYPE;

class IGameObject
{
public:
	IGameObject() { }
	~IGameObject() { }

	virtual void LoadFromFilePath(std::string filePath) = 0;

	virtual const void Load() = 0;
	virtual const void Unload() = 0;

	virtual const std::string& GetFilePath() const = 0;
	virtual void SetFilePath(const std::string& filePath) = 0;
};