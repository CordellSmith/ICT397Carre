#include "ResourceFactory.h"

bool ResourceFactory::CreateResource(RESOURCE_TYPE resourceType, std::string filePath)
{
	switch (resourceType)
	{
	case RESOURCE_MESH:
		m_resources.insert(std::pair<RESOURCE_TYPE, IResource*>(resourceType, MeshResource().CreateMeshFromFile(filePath)));
		return true;
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
		std::cout << "Unable to create Resource!" << std::endl;
		return false;
		break;
	}
}

IResource* ResourceFactory::GetResourceById(unsigned int id)
{
	for (std::map<unsigned int, IResource*>::iterator itr = m_resources.begin(); itr != m_resources.end(); ++itr)
	{
		if (itr->second->GetResourceId() == id)
			return itr->second;
	}
}

int ResourceFactory::TotalResources()
{
	int count = 0;
	for (std::map<unsigned int, IResource*>::iterator itr = m_resources.begin(); itr != m_resources.end(); ++itr)
	{
		count++;
	}

	return count;
}

const void ResourceFactory::Load()
{
	// TODO
	for (std::map<unsigned int, IResource*>::iterator itr = m_resources.begin(); itr != m_resources.end(); ++itr)
	{
		std::cout << itr->first << " => " << itr->second->IsLoaded() << '\n';
	}
	// Foreach resource in resource map -> load into engine
}

const void ResourceFactory::Unload()
{
	// TODO
	// Foreach resource loaded -> unload/destroy resource
}