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
	Resource* CreateResource(std::string filePath);

	Resource* GetResourceById(unsigned int id);
	int TotalResources();

	void Load() const;
	void Unload() const;

protected:
	// Stores vectors of all the same resources in a map with a corresponding key of RESOURCE_TYPE (see Resource.h)
	std::map<unsigned int, std::vector<Resource*>> resources;
};