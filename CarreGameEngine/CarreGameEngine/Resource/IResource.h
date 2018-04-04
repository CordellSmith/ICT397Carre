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
	// Default ctor has assigns a default id and type
	IResource() { }
	~IResource() { }

	// Virtual methods inherited from IResource interface to load resource from file
	virtual const std::string& GetFilePath() const = 0;
	virtual void SetFilePath(const std::string& filePath) = 0;

	// Used to identify a specific resource
	virtual const unsigned int& GetResourceId() const = 0;
	virtual void SetResourceId(unsigned int id) = 0;

	// Assigns the resource type
	virtual const RESOURCE_TYPE& GetResourceType() const = 0;
	virtual void SetResourceType(RESOURCE_TYPE type) = 0;

	// Checkers to see whether or not the resource has been loaded
	virtual bool IsLoaded() const = 0;
	virtual void SetLoaded(bool value) = 0;
};