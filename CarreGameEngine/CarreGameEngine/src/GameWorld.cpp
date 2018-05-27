#include "..\headers\GameWorld.h"
#include "..\headers\ComputerAI.h"

void GameWorld::Init(Player* player, std::multimap<std::string, IGameAsset*> gameAssets)
{
	// Sets this game contexts assets to the  loaded game assets from the control engine
	SetGameAssets(gameAssets);
	
	// Each new entity, asset needs its own shader (terrain or anything with multiple meshes especially)
	ShaderSource mainShader = ParseShaders("res/shaders/Test.shader");
	ShaderSource assimpShader = ParseShaders("res/shaders/Default.shader");
	ShaderSource terrainShader = ParseShaders("res/shaders/Terrain.shader");

	// Prepare terrains
	for each (Bruteforce* terrain in m_terrains)
	{
		terrain->SetCamera(m_camera);
		m_glRenderer.Prepare(terrain->GetModel(), terrainShader.VertexSource, terrainShader.FragmentSource);
	}
	
	// Prepare assets
	std::multimap<std::string, IGameAsset*>::iterator itr;
	for (itr = m_gameAssets.begin(); itr != m_gameAssets.end(); itr++)
	{
		//if (itr->first == "Cube")
		//{
			itr->second->SetCamera(m_camera);

			m_glRenderer.Prepare(itr->second->GetModel(), mainShader.VertexSource, mainShader.FragmentSource);
	
		if (itr->first == "md2")
		{
			itr->second->SetCamera(m_camera);
			m_glRenderer.Prepare(itr->second->GetModel(), mainShader.VertexSource, mainShader.FragmentSource);
		}
	}

	// Prepare player
	m_player = player;
	m_player->SetCamera(m_camera);
	m_glRenderer.Prepare(m_player->GetModel(), mainShader.VertexSource, mainShader.FragmentSource);

	// Pass player info to camera
	m_camera->PassPlayerInfo(m_player->GetPosition(), m_player->GetRotation());


	// AI testing
	/*ComputerAI p;
	for (int i = 0; i < 1000; i++)
		p.Update();
	getchar();*/
}

void GameWorld::Update()
{
	// Blue sky
	glClearColor(0.0, 0.0, 0.5, 1.0);

	// Render terrain
	for each (Bruteforce* terrain in m_terrains)
	{
		m_glRenderer.Render(terrain->GetModel());
	}

	// Render player
	m_glRenderer.Render(m_player->GetModel());
		

	// Prepare assets
	std::multimap<std::string, IGameAsset*>::iterator itr;
	for (itr = m_gameAssets.begin(); itr != m_gameAssets.end(); itr++)
	{
		// TODO: Not drawing lightpoles due to drop in framerate
		if (itr->first != "trafficLight")
		{
		m_glRenderer.Render(itr->second->GetModel());
		}
	}

	// Update all physics body locations *** All asset rendering is done through here for now because I dont want to have to call asset render twice ***
	UpdatePhysics();
}

void GameWorld::Destroy()
{
	std::multimap<std::string, IGameAsset*>::iterator itr;
	for (itr = m_gameAssets.begin(); itr != m_gameAssets.end(); itr++)
	{
		itr->second->Destroy();
	}

	// Delete all heightmap BruteForce
	for (int i = 0; i < m_terrains.size()-1; i++)
		delete m_terrains[i];
}

void GameWorld::SetPhysicsWorld(PhysicsEngine* physicsEngine, std::vector<btVector3> collisionBodyPositions)
{
	m_physicsWorld = physicsEngine;
	m_collisionBodyPos = collisionBodyPositions;
}

// Load all textures
void GameWorld::LoadAllTextures()
{
	// Use lua?
	// Or fstream?
}

// Update all physics
void GameWorld::UpdatePhysics()
{
	// Update physicsWorld
	// TODO: Make this better (Jack)
	glm::vec3 temp1(m_camera->GetPosition());
	btVector3 temp2(temp1.x, temp1.y, temp1.z);
	m_physicsWorld->Simulate(m_collisionBodyPos, temp2);

	float newY = m_terrains[0]->GetAverageHeight(m_camera->GetPosition().x, m_camera->GetPosition().z) + 50;

	// Set updated camera location
	m_camera->SetPosition(glm::vec3(m_camera->GetPosition().x, newY, m_camera->GetPosition().z));
		
	// Draw each object at the updated positions based on physics simulation
	std::multimap<std::string, IGameAsset*>::iterator itr;
	int i = 1;
	for (itr = m_gameAssets.begin(); itr != m_gameAssets.end(); itr++)
	{
		glm::vec3 temp = glm::vec3(m_collisionBodyPos[i].x(), m_terrains[0]->GetAverageHeight(m_collisionBodyPos[i].x(), m_collisionBodyPos[i].z()), m_collisionBodyPos[i].z());



		if (itr->first == "md2")
		{
			m_glRenderer.Render(itr->second->GetModel());
		}


		i++;
	}
}