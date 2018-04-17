#pragma once

#include <iostream>
#include <vector>

#include "IGameObject.h"

class GameAssetFactory
{
public:
	GameAssetFactory() { }
	~GameAssetFactory() { }

	IGameObject* CreateAsset(OBJ_TYPE type, std::string filePath);

protected:
};