#pragma once

#ifndef MD2_H
#define MD2_H

#include <iostream>
#include <windows.h>

#include "Model.h"

// These are the needed defines for the max values when loading .MD2 files
#define MD2_MAX_TRIANGLES		4096
#define MD2_MAX_VERTICES		2048
#define MD2_MAX_TEXCOORDS		2048
#define MD2_MAX_FRAMES			512
#define MD2_MAX_SKINS			32
#define MD2_MAX_FRAMESIZE		(MD2_MAX_VERTICES * 4 + 128)
#define kAnimationSpeed			5.0f

typedef char Md2Skin[64];

// This holds the header information that is read in at the beginning of the file
struct Md2Header
{
	int magic;					// This is used to identify the file
	int version;					// The version number of the file (Must be 8)
	int skinWidth;				// The skin width in pixels
	int skinHeight;				// The skin height in pixels
	int frameSize;				// The size in bytes the frames are
	int numSkins;				// The number of skins associated with the model
	int numVertices;				// The number of vertices (constant for each frame)
	int numTexCoords;			// The number of texture coordinates
	int numTriangles;			// The number of faces (polygons)
	int numGlCommands;			// The number of GL commands
	int numFrames;				// The number of animation frames
	int offsetSkins;				// The offset in the file for the skin data
	int offsetTexCoords;			// The offset in the file for the texture data
	int offsetTriangles;			// The offset in the file for the face data
	int offsetFrames;			// The offset in the file for the frames data
	int offsetGlCommands;		// The offset in the file for the GL commands data
	int offsetEnd;				// The end of the file offset
};

// This is used to store the vertices that are read in for the current frame
struct Md2AliasTriangle
{
	byte vertex[3];
	byte lightNormalIndex;
};

// This stores the normals and vertices for the frames
struct Md2Triangle
{
	float vertex[3];
	float normal[3];
};

// This stores the indices into the vertex and texture coordinate arrays
struct Md2Face
{
	short vertexIndices[3];
	short textureIndices[3];
};

// This stores UV coordinates
struct Md2TexCoord
{
	short u, v;
};

// This stores the animation scale, translation and name information for a frame, plus verts
struct Md2AliasFrame
{
	float scale[3];
	float translate[3];
	char name[16];
	Md2AliasTriangle aliasVertices[1];
};

// This stores the frames vertices after they have been transformed
struct Md2Frame
{
	char strName[16];
	Md2Triangle* pVertices;
};

struct AnimationInfo
{
	char strName[255];			// This stores the name of the animation (Jump, Pain, etc..)
	int startFrame;				// This stores the first frame number for this animation
	int endFrame;				// This stores the last frame number for this animation
};

class Md2
{
public:
	Md2();
	~Md2() { }

	bool ImportMD2(Model* model, const char* strFileName, std::string strTexture);
	void ReadMd2Data();
	void ParseAnimations(Model* md2Model);
	void ConvertDataStructures(Model* md2Model);

	void CleanUp() { }

protected:
	FILE* m_filePointer;

	Model* m_model;
	Md2Header m_header;			// The header data
	Md2Skin* m_skins;			// The skin data
	Md2TexCoord* m_texCoords;	// The texture coordinates
	Md2Face* m_triangles;		// Face index information
	Md2Frame* m_frames;			// The frames of animation (vertices)

};
#endif // !MD2_H
