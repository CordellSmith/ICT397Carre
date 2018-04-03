#pragma once

#include <iostream>

#include "IResource.h"

typedef enum 
{
	RESOURCE_NULL = 0,
	RESOURCE_MESH = 1,
	RESOURCE_MATERIAL = 2,
	RESOURCE_TERRAIN = 3,
	RESOURCE_TEXTURE = 4
}RESOURCE_TYPE;

class Resource : public IResource
{
public:
	Resource() : _resourceId(0), _type(RESOURCE_NULL) { }
	Resource(std::string filePath) : _resourceId(0), _type(RESOURCE_NULL) { }
	~Resource() { }

	// Virtual methods inherited from IResource interface to load resource from file
	virtual const std::string GetFilePath();
	virtual void SetFilePath(const std::string& filePath);

	// Used to identify a specific resource
	void SetResourceId(unsigned int id) { _resourceId = id; }
	unsigned int GetResourceId() const { return _resourceId; }

	// Assigns the resource type
	void SetResourceType(RESOURCE_TYPE type) { _type = type; }
	RESOURCE_TYPE GetResourceType() const { return _type; }

	// Checkers to see whether or not the resource has been loaded
	bool IsLoaded() const { return _loaded; }
	void SetLoaded(bool value) { _loaded = value; }

protected:
	unsigned int _resourceId;
	std::string _filePath;
	RESOURCE_TYPE _type;
	bool _loaded;
};