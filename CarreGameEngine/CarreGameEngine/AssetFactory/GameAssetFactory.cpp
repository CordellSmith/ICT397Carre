#include "GameAssetFactory.h"
#include "Terrain.h"
#include "Object.h"
#include "NPC.h"

IGameObject* GameAssetFactory::CreateAsset(OBJ_TYPE type, std::string filePath)
{
	switch (type)
	{
	case OBJ_TERRAIN:
		return new Terrain();
		break;
	case OBJ_OBJECT:
		break;
		return new Object();
	case OBJ_NPC:
		return new NPC();
	default:
		break;
	}
}
