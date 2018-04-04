#pragma once

#include <iostream>

#include "IResource.h"

class Resource : public IResource
{
public:
	// Default ctor has assigns a default id and type
	Resource() : m_resourceId(0), m_type(RESOURCE_NULL) { }
	~Resource() { }

	Resource(unsigned int resourceId) : m_resourceId(resourceId) { }
	Resource(RESOURCE_TYPE resourceType) : m_type(resourceType) { }

	// Virtual methods inherited from IResource interface to load resource from file
	virtual const std::string& GetFilePath() const;
	virtual void SetFilePath(const std::string& filePath);

	// Used to identify a specific resource
	unsigned int GetResourceId() const;
	void SetResourceId(unsigned int id);

	// Assigns the resource type
	RESOURCE_TYPE GetResourceType() const;
	void SetResourceType(RESOURCE_TYPE type);

	// Checkers to see whether or not the resource has been loaded
	bool IsLoaded() const { return m_loaded; }
	void SetLoaded(bool value) { m_loaded = value; }

protected:
	unsigned int m_resourceId;
	std::string m_filePath;
	RESOURCE_TYPE m_type;
	bool m_loaded;
};