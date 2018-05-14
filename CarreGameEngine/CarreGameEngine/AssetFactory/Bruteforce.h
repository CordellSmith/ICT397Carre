#pragma once

#include "NewTerrain.h"

class Bruteforce : public NewTerrain
{
public:
	Bruteforce() { }
	~Bruteforce() { }

	virtual void GenerateTerrain(GLuint textureId, std::string textureFilePath) override;
	virtual void AddShader(std::string vertShader, std::string fragShader) override;
	virtual void Render() override;

	bool LoadHeightfield(std::string fileName, const int size);
	Texture AddTexture(GLuint textureId, std::string filePath);
	bool Inbounds(int xpos, int zpos);
	float GetHeight(int xpos, int zpos);
	unsigned char GetHeightColour(int xpos, int zpos);
	float GetAverageHeight(int xpos, int zpos);

protected:
};