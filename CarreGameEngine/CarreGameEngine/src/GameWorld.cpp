#include "..\headers\GameWorld.h"

/// Function to read a shader in one string and return a struct containing separated Vertex and Fragment shaders
ShaderSource ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2]; // stack allocated array that will store vertex and fragment strings
	ShaderType type = ShaderType::NONE; // set to none (-1) by default
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

Vertex panel[6] = { glm::vec3(0), glm::vec4(1), glm::vec2(2), glm::vec3(3) };
std::vector<unsigned int> m_indices;
std::vector<Model> panelList;

// Prototyping
bool LoadHeightfield(char* filename, const int size);
bool Inbounds(int xpos, int zpos);
float GetHeight(int xpos, int zpos);
unsigned char GetHeightColour(int xpos, int zpos);
void RenderTerrain();

void GameWorld::Init(std::multimap<ASS_TYPE, IGameAsset*> gameAssets)
{
	// Sets this game contexts assets to the  loaded game assets from the control engine
	SetGameAssets(gameAssets);

	m_assimpShaderSource = ParseShader("res/shaders/Default.shader");
	m_testShaderSource = ParseShader("res/shaders/Test.shader");

	std::multimap<ASS_TYPE, IGameAsset*>::iterator itr;
	for (itr = m_gameAssets.begin(); itr != m_gameAssets.end(); itr++)
	{
		if (std::distance(m_gameAssets.begin(), itr) < 1)
		{
			// Pass camera pointer to all game objects to access projection / view matrices
			itr->second->SetCamera(m_camera);
			// Prepare shaders for each object (only testing with one model and shader atm)
			itr->second->Prepare(m_assimpShaderSource.VertexSource, m_assimpShaderSource.FragmentSource);
			continue;
		}

		itr->second->SetCamera(m_camera);
		itr->second->Prepare(m_testShaderSource.VertexSource, m_testShaderSource.FragmentSource);
	}

	PrepareColourPanel();
	m_colourPanel.Initialize(panel, 6, m_indices, m_shaderSource2.VertexSource, m_shaderSource2.FragmentSource);

	// Initialize all physics objects
	InitializePhysics();

	/*****************************************/
	// TextureManager testing
	TextureManager::Instance().LoadTexture("res/objects/taxi_model/taxi_chrome_d.dds");
	TextureManager::Instance().LoadTexture("res/objects/taxi_model/taxi_chrome_s.dds");
	TextureManager::Instance().LoadTexture("res/objects/taxi_model/taxi_d.dds");

	// This shouldn't work, as there is no file with that name
	TextureManager::Instance().LoadTexture("res/objects/taxi_model/taxi.dds");

	// This shouldn't work as .raw isn't supported with SOIL2
	TextureManager::Instance().LoadTexture("res/terrain/height128.raw");

	RenderTerrain();

}

void GameWorld::Update()
{
	// Blue sky
	glClearColor(0.0, 0.0, 0.5, 1.0);

	//// Render a floor on x/z axis (15 x 15)
	//for (int x = 1; x < 11; x++)
	//{
	//	for (int z = 1; z < 11; z++)
	//	{
	//		m_colourPanel.SetPosition(glm::vec3(x * 5, -1.0, z * 5));
	//		m_colourPanel.Render();
	//	}
	//}

	// Update all physics body locations
	UpdatePhysics();

	// Testing rendering of objects from multimap
	std::multimap<ASS_TYPE, IGameAsset*>::iterator itr;
	for (itr = m_gameAssets.begin(); itr != m_gameAssets.end(); itr++)
	{
		itr->second->Render();
	}
}

void GameWorld::Destroy()
{
	/// Delete all textures
	TextureManager::Instance().ReleaseAllTextures();

	m_colourPanel.Destroy();

	std::multimap<ASS_TYPE, IGameAsset*>::iterator itr;
	for (itr = m_gameAssets.begin(); itr != m_gameAssets.end(); itr++)
	{
		itr->second->Destroy();
	}
}

// Initialize all physics
void GameWorld::InitializePhysics()
{
	// Create camera rigid body
	glm::vec3 tempCam(m_camera->GetPosition());
	btVector3 tempCam2(tempCam.x, tempCam.y, tempCam.z);
	m_physicsWorld.CreatePlayerControlledRigidBody(tempCam2);
	m_collisionBodyPos.push_back(tempCam2);

	// Create static rigid body (floor)
	m_physicsWorld.CreateStaticRigidBody();
	m_collisionBodyPos.push_back(btVector3(0.0, 0.0, 0.0));

	// Create 15 randomly places dynamic rigid bodies
	for (int i = 0; i < 15; i++)
	{
		if (i == 0)
		{
			m_physicsWorld.CreateDynamicRigidBody(btVector3(15.0, 15.0, 15.0));
			m_collisionBodyPos.push_back(btVector3(15.0, 15.0, 15.0));
			continue;
		}
		m_physicsWorld.CreateDynamicRigidBody(btVector3(rand() % 30, 0.0, rand() % 30));
		m_collisionBodyPos.push_back(btVector3(rand() % 30, 0.0, rand() % 30));
	}

	// Create heightmap terrain shape
	//physicsWorld.CreateHeightfieldTerrainShape();
	//collisionBodyPos.push_back(btVector3(0.0, 0.0, 0.0));

	// Activate all rigid body objects
	m_physicsWorld.ActivateAllObjects();
}

// Update all physics
void GameWorld::UpdatePhysics()
{
	// Update physicsWorld
	// TODO: Make this better (Jack)
	glm::vec3 temp1(m_camera->GetPosition());
	btVector3 temp2(temp1.x, temp1.y, temp1.z);
	m_physicsWorld.Simulate(m_collisionBodyPos, temp2);

	// Set updated camera location
	m_camera->SetPosition(glm::vec3(temp2.getX(), temp2.getY(), temp2.getZ()));

	// Draw shapes for testing (just planes atm, didn't know how to make sphem_assetType using current setup)
	//glm::vec3 temp = glm::vec3(m_collisionBodyPos[0].x(), m_collisionBodyPos[0].y(), m_collisionBodyPos[0].z());
	//colourPanel.SetPosition(glm::vec3(temp.x, temp.y, temp.z));

	// Draw each object at the updated positions based on physics simulation
	for (size_t i = 0; i < m_collisionBodyPos.size(); i++)
	{
		glm::vec3 temp = glm::vec3(m_collisionBodyPos[i].x(), m_collisionBodyPos[i].y(), m_collisionBodyPos[i].z());

		// First collision body is camera, second is the the static ground plane, so draw a colour panel instead of a car
		if (i == 0 || i == 1)
		{
			m_colourPanel.SetPosition(glm::vec3(temp.x, temp.y, temp.z));
			m_colourPanel.Render();
			continue;
		}
		// This code is trash hahah :P, have to find a way to select which object you want from the multimap. Will have to
		// use some sort of search function as a string parameter like "cube" for instance then it be returned using the
		// itr->second.
		std::multimap<ASS_TYPE, IGameAsset*>::iterator itr = m_gameAssets.begin();
		itr->second->SetAssetPosition(glm::vec3(temp.x, temp.y, temp.z));
		itr->second->Render();
	}
}

int g_size;
int scaleX = 5;
int scaleY = 1;
int scaleZ = 5;
unsigned char* terrainData;

bool LoadHeightfield(char* filename, const int size)
{
	// Open for binary read, print error if error found
	std::ifstream infile(filename, std::ios::binary);
	if (!infile)
	{
		std::cerr << "Cannot open file :" << filename << std::endl;
		return false;
	}

	// Allocate memory, return false if no size = 0
	if (terrainData)
		delete[] terrainData;
	if (size > 0)
		terrainData = new unsigned char[size * size];

	// Read in heightfield and get length of file
	infile.seekg(0, std::ios::end);
	int length = infile.tellg();

	// Read data in as a block, cast to char*, set size, and close file
	infile.seekg(0, std::ios::beg);
	infile.read(reinterpret_cast<char *>(terrainData), length);
	infile.close();
	g_size = size;

	return true;
}

bool Inbounds(int xpos, int zpos)
{
	if ((xpos >= 0 && xpos < g_size) && (zpos >= 0 && zpos < g_size))
		return true;
	else
		return false;
}

float GetHeight(int xpos, int zpos)
{
	if (Inbounds(xpos, zpos))
	{
		return ((float)(terrainData[(zpos * g_size) + xpos])*scaleY);
	}
	return 1;
}

unsigned char GetHeightColour(int xpos, int zpos)
{
	if (Inbounds(xpos, zpos))
	{
		return terrainData[zpos * g_size + xpos];
	}
	return 1;
}

void RenderTerrain()
{
	// Colour of the height
	unsigned char hcolor;

	//
	float texLeft, texBottom, texTop;

	//if (tex)
	//{
	//	glEnable(GL_TEXTURE_2D);
	//	glBindTexture(GL_TEXTURE_2D, tex);
	//}
	// Loop through the z-axis
	for (int z = 0; z < g_size - 1; z++)
	{
		// Loop through the x-axis
		glBegin(GL_TRIANGLE_STRIP);
		for (int x = 0; x < g_size; x++)
		{
			//calculate the texture coordinates
			texLeft = (float)x / g_size;
			texBottom = (float)z / g_size;
			texTop = (float)(z + 1) / g_size;

			// Create the the first point in the triangle strip
			hcolor = GetHeightColour(x, z);
			glColor3ub(hcolor, hcolor, hcolor);
			glTexCoord2f(texLeft, texBottom);
			glVertex3f((float)x * scaleX, GetHeight(x, z), (float)z * scaleZ);

			// Create the next point in the triangle strip
			hcolor = GetHeightColour(x, z + 1);
			glColor3ub(hcolor, hcolor, hcolor);
			glTexCoord2f(texLeft, texTop);
			glVertex3f((float)x * scaleX, GetHeight(x, z + 1), (float)(z + 1) * scaleZ);
		}
		glEnd();
	}
}

void GameWorld::PrepareColourPanel()
{
	m_shaderSource2 = ParseShader("res/shaders/Basic.shader");
	m_colourPanel.SetCamera(m_camera);

	if (!LoadHeightfield("res/terrain/height128.raw", 128))
		std::cout << "ERROR!" << std::endl;

	panel[0].xyz = glm::vec3(-5.0f, -5.0f, -5.0f);
	panel[0].rgba = glm::vec4(1.0, 1.0, 1.0, 1.0);

	panel[1].xyz = glm::vec3(5.0f, -5.0f, -5.0f);
	panel[1].rgba = glm::vec4(1.0, 1.0, 1.0, 1.0);

	panel[2].xyz = glm::vec3(5.0f, -5.0f, 5.0f);
	panel[2].rgba = glm::vec4(1.0, 1.0, 1.0, 1.0);

	panel[3].xyz = glm::vec3(5.0f, -5.0f, 5.0f);
	panel[3].rgba = glm::vec4(1.0, 1.0, 1.0, 1.0);

	panel[4].xyz = glm::vec3(-5.0f, -5.0f, 5.0f);
	panel[4].rgba = glm::vec4(1.0, 1.0, 1.0, 1.0);

	panel[5].xyz = glm::vec3(-5.0f, -5.0f, -5.0f);
	panel[5].rgba = glm::vec4(1.0, 1.0, 1.0, 1.0);

	for (int i = 1; i < 7; i++)
		m_indices.push_back(i);
}

