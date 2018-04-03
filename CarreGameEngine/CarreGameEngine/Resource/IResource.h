#pragma once

#include <iostream>

class IResource
{
public:
	IResource() { }
	~IResource() { }

	virtual const std::string GetFilePath() const = 0;
	virtual void SetFilePath(const std::string& filePath) = 0;
};