#pragma once

#include "Resource.h"

const std::string& Resource::GetFilePath() const
{
	return m_filePath;
}

void Resource::SetFilePath(const std::string& filePath)
{
	m_filePath = filePath;
}

unsigned int Resource::GetResourceId() const
{
	return m_resourceId;
}

void Resource::SetResourceId(unsigned int id)
{
	m_resourceId = id;
}

RESOURCE_TYPE Resource::GetResourceType() const
{
	return m_type;
}

void Resource::SetResourceType(RESOURCE_TYPE type)
{
	m_type = type;
}