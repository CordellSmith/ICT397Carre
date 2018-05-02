#include "GameAssetFactory.h"
#include "Terrain.h"
#include "Object.h"
#include "NPC.h"

IGameAsset* GameAssetFactory::CreateAsset(ASS_TYPE type, std::string assetName)
{
	switch (type)
	{
	case ASS_TERRAIN:
		return new Terrain(assetName);
		break;
	case ASS_OBJECT:
		return new Object(assetName);
		break;
	case ASS_NPC:
		return new NPC(assetName);
		break;
	default:
		return nullptr;
		break;
	}
}

const void GameAssetFactory::AddAsset(IGameAsset* assetToAdd)
{
	std::pair<ASS_TYPE, IGameAsset*> tempAsset = std::pair<ASS_TYPE, IGameAsset*>(assetToAdd->GetOBJType(), assetToAdd);
	m_assets.insert(tempAsset);
}

