#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "IResource.h"
#include "MeshResource.h"

class ResourceFactory
{
public:
	ResourceFactory() { }
	~ResourceFactory() { }

	// Todo: Implement these functions
	bool CreateResource(RESOURCE_TYPE resourceType, std::string filePath);

	IResource* GetResourceById(unsigned int id);

	int TotalResources();

	// This will load all the resources from external files (similar to init)
	const void Load();
	// This will unload all the resources for memory management (similar to delete/destroy)
	const void Unload();

protected:
	std::map<unsigned int, IResource*> m_resources;
	// Stores vectors of all the same resources in a map with a corresponding key of RESOURCE_TYPE (see Resource.h)
};