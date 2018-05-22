#pragma once

#include "Terrain.h"

class Bruteforce : public Terrain
{
public:
	Bruteforce(float scaleX, float scaleY, float scaleZ);
	~Bruteforce() { }

	virtual void GenerateTerrain(GLuint textureId, std::string textureFilePath) override;

	bool LoadHeightfield(std::string fileName, const int size);
	Texture AddTexture(GLuint textureId, std::string filePath);
	bool Inbounds(int xpos, int zpos);
	float GetHeight(int xpos, int zpos);
	unsigned char GetHeightColour(int xpos, int zpos);
	float GetAverageHeight(int xpos, int zpos);

protected:
};