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

void GameWorld::Init(std::multimap<ASS_TYPE, IGameAsset*> gameAssets)
{
	// Sets this game contexts assets to the  loaded game assets from the control engine
	SetGameAssets(gameAssets);

	m_assimpShaderSource = ParseShader("res/shaders/Default.shader");

	std::multimap<ASS_TYPE, IGameAsset*>::iterator itr;
	for (itr = m_gameAssets.begin(); itr != m_gameAssets.end(); itr++)
	{
		// Pass camera pointer to all game objects to access projection / view matrices
		itr->second->SetCamera(m_camera);
		// Prepare shaders for each object (only testing with one model and shader atm)
		itr->second->Prepare(m_assimpShaderSource.VertexSource, m_assimpShaderSource.FragmentSource);
	}

	PrepareColourPanel();
	m_colourPanel.Initialize(panel, 6, m_indices, m_shaderSource2.VertexSource, m_shaderSource2.FragmentSource);

	// Initialize all physics objects
	InitializePhysics();
}

void GameWorld::Update()
{
	// Update all physics body locations
	UpdatePhysics();

	// Render a floor on x/z axis (15 x 15)
	for (int x = 0; x < 15; x++)
	{
		for (int z = 0; z < 15; z++)
		{
			m_colourPanel.SetPosition(glm::vec3(x * 2, 0, z * 2));
			m_colourPanel.Render();
		}
	}

	// Testing rendering of objects from multimap
	std::multimap<ASS_TYPE, IGameAsset*>::iterator itr;
	for (itr = m_gameAssets.begin(); itr != m_gameAssets.end(); itr++)
	{
		itr->second->Render();
	}
}

void GameWorld::Destroy()
{
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

	// Draw shapes for testing (just planes atm, didn't know how to make spheres using current setup)
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
		itr->second->SetObjectPosition(glm::vec3(temp.x, temp.y, temp.z));
		itr->second->Render();
	}
}

void GameWorld::PrepareColourPanel()
{
	m_shaderSource2 = ParseShader("res/shaders/Basic.shader");
	m_colourPanel.SetCamera(m_camera);

	panel[0].xyz = glm::vec3(-1.0f, -1.0f, -1.0f);
	panel[0].rgba = glm::vec4(0.5, 0.5, 0.0, 1);

	panel[1].xyz = glm::vec3(1.0f, -1.0f, -1.0f);
	panel[1].rgba = glm::vec4(0.4, 0.0, 1.0, 1);

	panel[2].xyz = glm::vec3(1.0f, -1.0f, 1.0f);
	panel[2].rgba = glm::vec4(0.5, 0.6, 1.0, 1);

	panel[3].xyz = glm::vec3(1.0f, -1.0f, 1.0f);
	panel[3].rgba = glm::vec4(1.0, 0.4, 0.4, 1);

	panel[4].xyz = glm::vec3(-1.0f, -1.0f, 1.0f);
	panel[4].rgba = glm::vec4(0.3, 1.0, 0.1, 1);

	panel[5].xyz = glm::vec3(-1.0f, -1.0f, -1.0f);
	panel[5].rgba = glm::vec4(1.0, 1.0, 0.4, 1);

	for (int i = 1; i < 7; i++)
		m_indices.push_back(i);
}
