#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "IResource.h"
#include "Mesh.h"

class ResourceFactory
{
public:
	ResourceFactory() { }
	~ResourceFactory() { }

	// Todo: Implement these functions
	void CreateResource(RESOURCE_TYPE resourceType, std::string filePath);

	IResource* GetResourceById(unsigned int id);

	int TotalResources();

	// This will load all the resources from external files (similar to init)
	void Load() const;
	// This will unload all the resources for memory management (similar to delete/destroy)
	void Unload() const;

	void Print()
	{
		for (std::map<unsigned int, IResource*>::iterator itr = m_resources.begin(); itr != m_resources.end(); ++itr)
			std::cout << itr->first << " => " << itr->second << '\n';
	}

protected:
	// Stores vectors of all the same resources in a map with a corresponding key of RESOURCE_TYPE (see Resource.h)
	std::map<unsigned int, IResource*> m_resources;
};