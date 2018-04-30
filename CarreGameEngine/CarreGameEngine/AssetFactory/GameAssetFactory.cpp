#include "GameAssetFactory.h"
#include "Terrain.h"
#include "Object.h"
#include "NPC.h"

IGameObject* GameAssetFactory::CreateAsset(OBJ_TYPE type, std::string filePath)
{
	switch (type)
	{
	case OBJ_TERRAIN:
		return new Terrain(filePath);
		break;
	case OBJ_OBJECT:
		return new Object(filePath);
		break;
	case OBJ_NPC:
		return new NPC(filePath);
		break;
	default:
		break;
	}
}

const void GameAssetFactory::AddAsset(IGameObject* assetToAdd)
{
	std::pair<OBJ_TYPE, IGameObject*> tempAsset = std::pair<OBJ_TYPE, IGameObject*>(assetToAdd->GetOBJType(), assetToAdd);
	m_assets.insert(tempAsset);
}

