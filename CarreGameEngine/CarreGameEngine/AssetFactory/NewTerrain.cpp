#include "NewTerrain.h"

NewTerrain::NewTerrain()
	: m_scaleX(10.0), m_scaleY(1.0), m_scaleZ(10.0), m_heightfieldSize(0)
{
	m_terrainModel = new NewModel();
}

NewTerrain::NewTerrain(float scaleX, float scaleY, float scaleZ) 
	: m_scaleX(scaleX), m_scaleY(scaleY), m_scaleZ(scaleZ), m_heightfieldSize(0)
{
	m_terrainModel = new NewModel();
}