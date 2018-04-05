#include "ResourceFactory.h"

void ResourceFactory::CreateResource(RESOURCE_TYPE resourceType, std::string filePath)
{
	IResource* resource;
	
	switch (resourceType)
	{
	case RESOURCE_MESH:
		resource = new Mesh(filePath);
		m_resources.insert(std::pair<RESOURCE_TYPE, IResource*>(resourceType, resource));
		break;
	//case RESOURCE_MATERIAL:
	//	return new Material;
	//	break;
	//case RESOURCE_TERRAIN:
	//	return new Terrain;
	//	break;
	//case RESOURCE_TEXTURE:
	//	return new Texture;
	//	break;
	default:
		break;
	}
}

IResource* ResourceFactory::GetResourceById(unsigned int id)
{
	return nullptr;
}

int ResourceFactory::TotalResources()
{
	return 0;
}

void ResourceFactory::Load() const
{
	// TODO
	// Foreach resource in resource map -> load into engine
}

void ResourceFactory::Unload() const
{
	// TODO
	// Foreach resource loaded -> unload/destroy resource
}