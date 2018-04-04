#include "ResourceFactory.h"

IResource* ResourceFactory::CreateResource(RESOURCE_TYPE resourceType, std::string filePath)
{
	switch (resourceType)
	{
	case RESOURCE_MESH:
		return new Mesh(filePath);
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
		return NULL;
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