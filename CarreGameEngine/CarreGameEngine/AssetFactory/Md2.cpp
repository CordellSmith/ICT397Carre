#include "Md2.h"

Md2::Md2()
{
	memset(&m_header, 0, sizeof(Md2Header));
	m_skins = NULL;			
	m_texCoords = NULL;		
	m_triangles = NULL;		
	m_frames = NULL;
}

bool Md2::ImportMD2(Model* model, const char* strFileName, std::string strTexture)
{
	std::string strMessage;

	// Open the MD2 file in binary
	//m_filePointer = std::fopen(strFileName, "rb");

	// Make sure we have a valid file pointer (we found the file)
	if (!m_filePointer)
	{
		// Display an error message and don't load anything if no file was found
		std::cout << "Unable to find the file: " << strFileName << std::endl;
		return false;
	}

	// Read the header data and store it in our m_Header member variable
	std::fread(&m_header, 1, sizeof(Md2Header), m_filePointer);

	// Make sure the version is this crazy number '8' or else it's a bad egg
	if (m_header.version != 8)
	{
		// Display an error message for bad file format, then stop loading.
		std::cout << "Invalid file format (Version not 8): " << strFileName << std::endl;
		return false;
	}

	// Read in the model and animation data
	ReadMd2Data();

	// Here we pass in our model structure so it can store and read Quake data
	// in our own model and object structure data.
	//ConvertDataStructures(model);

	CleanUp();

	return true;
}

void Md2::ReadMd2Data()
{
	// Create a larger buffer for the frames of animation (not fully used yet)
	unsigned char buffer[MD2_MAX_FRAMESIZE];

	// Here we allocate all of our memory from the header's information
	m_skins = new Md2Skin[m_header.numSkins];
	m_texCoords = new Md2TexCoord[m_header.numTexCoords];
	m_triangles = new Md2Face[m_header.numTriangles];
	m_frames = new Md2Frame[m_header.numFrames];

	// Next, we start reading in the data by seeking to our skin names offset
	fseek(m_filePointer, m_header.offsetSkins, SEEK_SET);

	// Depending on the skin count, we read in each skin for this model
	fread(m_skins, sizeof(Md2Skin), m_header.numSkins, m_filePointer);

	// Move the file pointer to the position in the file for texture coordinates
	fseek(m_filePointer, m_header.offsetTexCoords, SEEK_SET);

	// Read in all the texture coordinates in one fell swoop
	fread(m_texCoords, sizeof(Md2TexCoord), m_header.numTexCoords, m_filePointer);

	// Move the file pointer to the triangles/face data offset
	fseek(m_filePointer, m_header.offsetTriangles, SEEK_SET);

	// Read in the face data for each triangle (vertex and texCoord indices)
	fread(m_triangles, sizeof(Md2Face), m_header.numTriangles, m_filePointer);

	// Move the file pointer to the vertices (frames)
	fseek(m_filePointer, m_header.offsetFrames, SEEK_SET);

	// In the last tutorial we just read in one frame of animation here.  Now we are going
	// to extract every key frame from the .MD2 model.  These key frames will be used to
	// interpolate between each other to form the somewhat smooth animation.
	// The only thing different is that we are putting a for loop in front, then changing
	// the '0' for the m_pFrames[0] in the last tutorial, to an 'i'.  The loop will
	// continue until 'i' has reached the number of key frames for this model.

	for (int i = 0; i < m_header.numFrames; i++)
	{
		// Assign our alias frame to our buffer memory
		Md2AliasFrame* pFrame = (Md2AliasFrame *)buffer;

		// Allocate the memory for the first frame of animation's vertices
		m_frames[i].pVertices = new Md2Triangle[m_header.numVertices];

		// Read in the first frame of animation
		fread(pFrame, 1, m_header.frameSize, m_filePointer);

		// Copy the name of the animation to our frames array
		//strcpy(m_frames[i].strName, pFrame->name);

		// Store off a vertex array pointer to cut down large lines of code
		Md2Triangle* pVertices = m_frames[i].pVertices;

		// Go through all of the number of vertices and assign the scale and translations.
		// Store the vertices in our current frame's vertex list array, while swapping Y and Z.
		// Notice we also negate the Z axis as well to make the swap correctly.
		for (int j = 0; j < m_header.numVertices; j++)
		{
			pVertices[j].vertex[0] = pFrame->aliasVertices[j].vertex[0] * pFrame->scale[0] + pFrame->translate[0];
			pVertices[j].vertex[2] = -1 * (pFrame->aliasVertices[j].vertex[1] * pFrame->scale[1] + pFrame->translate[1]);
			pVertices[j].vertex[1] = pFrame->aliasVertices[j].vertex[2] * pFrame->scale[2] + pFrame->translate[2];
		}
	}
}

/*
void Md2::ParseAnimations(Model* model)
{
	for (int i = 0; i < model->numOfObjects; i++)
	{
		// Assign the name of this frame of animation to a string object
		string strName = m_pFrames[i].strName;
		int frameNum = 0;

		// Go through and extract the frame numbers and erase them from the name
		for (unsigned int j = 0; j < strName.length(); j++)
		{
			// If the current index is a number and it's one of the last 2 characters of the name
			if (isdigit(strName[j]) && j >= strName.length() - 2)
			{
				// Use a C function to convert the character to an integer.
				// Notice we use the address to pass in the current character and on
				frameNum = atoi(&strName[j]);

				// Erase the frame number from the name so we extract the animation name
				strName.erase(j, strName.length() - j);
				break;
			}
		}

		// Check if this animation name is not the same as the last frame,
		// or if we are on the last frame of animation for this model
		if (strName != strLastName || i == model->numOfObjects - 1)
		{
			// If this animation frame is NOT the first frame
			if (strLastName != "")
			{
				// Copy the last animation name into our new animation's name
				strcpy(animation.strName, strLastName.c_str());

				// Set the last frame of this animation to i
				animation.endFrame = i;

				// Add the animation to our list and reset the animation object for next time
				pModel->pAnimations.push_back(animation);
				memset(&animation, 0, sizeof(tAnimationInfo));

				// Increase the number of animations for this model
				pModel->numOfAnimations++;
			}

			// Set the starting frame number to the current frame number we just found,
			// minus 1 (since 0 is the first frame) and add 'i'.
			animation.startFrame = frameNum - 1 + i;
		}

		// Store the current animation name in the strLastName string to check it later
		strLastName = strName;
	}
}

void Md2::ConvertDataStructures(Model* model)
{
	// CONTINUE FROM HERE!
	// Initialize the model structure first before assigning data to it
	memset(model, 0, sizeof(model->GetMeshBatch().size()));

	// Set the number of objects to our number of frames since pObjects will
	// hold the key frames.
	pModel->numOfObjects = m_header.numFrames;

	// Create our animation list and store it in our model
	ParseAnimations(pModel);

	// Go through every key frame and store it's vertices info in our pObject list.
	for (i = 0; i < pModel->numOfObjects; i++)
	{
		// Create a local object to store the first frame of the animation's data
		t3DObject currentFrame = { 0 };

		// Assign the vertex, texture coord and face count to our new structure
		currentFrame.numOfVerts = m_Header.numVertices;
		currentFrame.numTexVertex = m_Header.numTexCoords;
		currentFrame.numOfFaces = m_Header.numTriangles;

		// Allocate memory for the vertices, texture coordinates and face data.
		currentFrame.pVerts = new CVector3[currentFrame.numOfVerts];

		// Go through all of the vertices and assign them over to our structure
		for (j = 0; j < currentFrame.numOfVerts; j++)
		{
			currentFrame.pVerts[j].x = m_pFrames[i].pVertices[j].vertex[0];
			currentFrame.pVerts[j].y = m_pFrames[i].pVertices[j].vertex[1];
			currentFrame.pVerts[j].z = m_pFrames[i].pVertices[j].vertex[2];
		}

		// We can now free the old vertices stored in this frame of animation
		// since we have them stored in the pObject list of our model.
		delete m_pFrames[i].pVertices;

		// Now comes the tricky part, since we only need to store the UV coordinates
		// and face indices once, we only do this on the first frame.  If the
		// current frame is past the first frame, we just add the current frame
		// as it is to our list, then continue to the next frame.  If we are
		// on the first frame, we still need to store the UV and face information.
		// This will only go in the first frame, and will be referenced from the
		// remaining frames when we animate the model.

		// Check if we are past the first key frame
		if (i > 0)
		{
			// Here we add the current object (or frame) to our list object list
			pModel->pObject.push_back(currentFrame);
			continue;	// Go on to the next key frame
		}

		// We will only get here ONE because we just need this information
		// calculated for the first key frame.

		// Allocate memory for our UV coordinates and face information
		currentFrame.pTexVerts = new CVector2[currentFrame.numTexVertex];
		currentFrame.pFaces = new tFace[currentFrame.numOfFaces];

		// Go through all of the uv coordinates and assign them over to our structure.
		// The UV coordinates are not normal uv coordinates, they have a pixel ratio of
		// 0 to 256.  We want it to be a 0 to 1 ratio, so we divide the u value by the
		// skin width and the v value by the skin height.  This gives us our 0 to 1 ratio.
		// For some reason also, the v coordinate is flipped upside down.  We just subtract
		// the v coordinate from 1 to remedy this problem.
		for (j = 0; j < currentFrame.numTexVertex; j++)
		{
			currentFrame.pTexVerts[j].x = m_pTexCoords[j].u / float(m_Header.skinWidth);
			currentFrame.pTexVerts[j].y = 1 - m_pTexCoords[j].v / float(m_Header.skinHeight);
		}

		// Go through all of the face data and assign it over to OUR structure
		for (j = 0; j < currentFrame.numOfFaces; j++)
		{
			// Assign the vertex indices to our face data
			currentFrame.pFaces[j].vertIndex[0] = m_pTriangles[j].vertexIndices[0];
			currentFrame.pFaces[j].vertIndex[1] = m_pTriangles[j].vertexIndices[1];
			currentFrame.pFaces[j].vertIndex[2] = m_pTriangles[j].vertexIndices[2];

			// Assign the texture coord indices to our face data
			currentFrame.pFaces[j].coordIndex[0] = m_pTriangles[j].textureIndices[0];
			currentFrame.pFaces[j].coordIndex[1] = m_pTriangles[j].textureIndices[1];
			currentFrame.pFaces[j].coordIndex[2] = m_pTriangles[j].textureIndices[2];
		}

		// Here we add the current object (or frame) to our list object list
		pModel->pObject.push_back(currentFrame);
	}
}
*/