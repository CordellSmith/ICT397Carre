#pragma once

#include <iostream>

typedef enum
{
	RESOURCE_NULL = 0,
	RESOURCE_MESH = 1,
	RESOURCE_MATERIAL = 2,
	RESOURCE_TERRAIN = 3,
	RESOURCE_TEXTURE = 4
}RESOURCE_TYPE;

class IResource
{
public:
	IResource() { }
	~IResource() { }

	virtual const std::string& GetFilePath() const = 0;
	virtual void SetFilePath(const std::string& filePath) = 0;
};