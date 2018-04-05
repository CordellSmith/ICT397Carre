#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "Resource.h"

class ResourceFactory
{
public:
	ResourceFactory() { }
	~ResourceFactory() { }

	// Implement these
	Resource* CreateResource(RESOURCE_TYPE resourceType);

	Resource* GetResourceById(unsigned int id);
	int TotalResources();

	// This will load all the resources from external files (similar to init)
	void Load() const;
	// This will unload all the resources for memory management (similar to delete/destroy)
	void Unload() const;

protected:
	// Stores vectors of all the same resources in a map with a corresponding key of RESOURCE_TYPE (see Resource.h)
	std::map<unsigned int, std::vector<Resource*>> m_resources;
};