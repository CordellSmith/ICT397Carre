#include "Bruteforce.h"

void Bruteforce::GenerateTerrain()
{
	Mesh tempMesh;

	// Colour of the height
	//unsigned char hcolor;
	//float texLeft, texBottom, texTop;

	Vertex3 vertex;

	//if (tex)
	//{
	//	glEnable(GL_TEXTURE_2D);
	//	glBindTexture(GL_TEXTURE_2D, tex);
	//}
	// Loop through the z-axis
	for (int z = 0; z < m_heightfieldSize - 1; z++)
	{
		// Loop through the x-axis
		//glBegin(GL_TRIANGLE_STRIP);
		for (int x = 0; x < m_heightfieldSize; x++)
		{
			vertex.m_texCoords.x = (float)x / m_heightfieldSize;
			vertex.m_texCoords.y = (float)z / m_heightfieldSize;
			//calculate the texture coordinates
			//texLeft = (float)x / m_heightfieldSize;
			//texBottom = (float)z / m_heightfieldSize;
			//texTop = (float)(z + 1) / m_heightfieldSize;
			//glTexCoord2f(texLeft, texBottom);

			// Create the the first point in the triangle strip
			//hcolor = GetHeightColour(x, z);
			//glColor3ub(hcolor, hcolor, hcolor);

			vertex.m_position.x = (float)x * m_scaleX;
			vertex.m_position.y = GetHeight(x, z);
			vertex.m_position.z = (float)z * m_scaleZ;

			vertex.m_colour = glm::vec4(((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)), 1.0);
			//glVertex3f((float)x * m_scaleX, GetHeight(x, z), (float)z * m_scaleZ);

			tempMesh.GetVertices().push_back(vertex);

			// Create the next point in the triangle strip
			//hcolor = GetHeightColour(x, z + 1);
			//glColor3ub(hcolor, hcolor, hcolor);

			//glTexCoord2f(texLeft, texTop);
			//glVertex3f((float)x * m_scaleX, GetHeight(x, z + 1), (float)(z + 1) * m_scaleZ);
		}
		//glEnd();
	}
	tempMesh.SetupMesh();
	m_terrainModel->GetMeshBatch().push_back(tempMesh);
	m_terrainModel->SetScale(glm::vec3(1.0, 1.0, 1.0));
}

void Bruteforce::AddShader(std::string vertShader, std::string fragShader)
{
	m_terrainModel->Prepare(vertShader, fragShader);
}

void Bruteforce::Render()
{
	m_terrainModel->Draw();
}

bool Bruteforce::LoadHeightfield(std::string fileName, const int size)
{
	// Open for binary read, print error if error found
	std::ifstream infile(fileName.c_str(), std::ios::binary);
	if (!infile)
	{
		std::cout << "Cannot open file: " << fileName << std::endl;
		return false;
	}

	// Allocate memory, return false if no size = 0
	if (m_terrainData)
		delete[] m_terrainData;
	if (size > 0)
		m_terrainData = new unsigned char[size * size];

	// Read in heightfield and get length of file
	infile.seekg(0, std::ios::end);
	int length = infile.tellg();

	// Read data in as a block, cast to char*, set size, and close file
	infile.seekg(0, std::ios::beg);
	infile.read(reinterpret_cast<char*>(m_terrainData), length);
	infile.close();
	m_heightfieldSize = size;
	
	std::cout << "Heightfield Load Successful!" << std::endl;
	
	return true;
}

bool Bruteforce::Inbounds(int xpos, int zpos)
{
	if ((xpos >= 0 && xpos < m_heightfieldSize) && (zpos >= 0 && zpos < m_heightfieldSize))
		return true;
	else
		return false;
}

float Bruteforce::GetHeight(int xpos, int zpos)
{
	if (Inbounds(xpos, zpos))
	{
		return ((float)(m_terrainData[(zpos * m_heightfieldSize) + xpos]) * m_scaleY);
	}
	return 1;
}

unsigned char Bruteforce::GetHeightColour(int xpos, int zpos)
{
	if (Inbounds(xpos, zpos))
	{
		return m_terrainData[zpos * m_heightfieldSize + xpos];
	}
	return 1;
}
