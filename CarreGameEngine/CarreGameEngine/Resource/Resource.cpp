#pragma once

#include "Resource.h"

Resource::Resource(std::string filePath) : _resourceId(0), _type(RESOURCE_NULL)
{ 
	SetFilePath(filePath);
}

const std::string Resource::GetFilePath()
{
	return _filePath;
}

void Resource::SetFilePath(const std::string& filePath)
{
	_filePath = filePath;
}