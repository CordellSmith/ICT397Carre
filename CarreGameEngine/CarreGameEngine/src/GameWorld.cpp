#include "..\headers\GameWorld.h"

void GameWorld::Init(Player* player, std::multimap<std::string, IGameAsset*> gameAssets)
{
	// Set player
	m_player = player;
	// Sets this game contexts assets to the  loaded game assets from the control engine
	SetGameAssets(gameAssets);
	// Pass camera to terrain
	m_terrain.SetCamera(m_camera);
	// Pass camera to player
	m_player->SetCamera(m_camera);

	std::multimap<std::string, IGameAsset*>::iterator itr;
	for (itr = m_gameAssets.begin(); itr != m_gameAssets.end(); itr++)
	{
		if (itr->first == "Cube")
		{
			// Pass camera pointer to all game objects to access projection / view matrices
			itr->second->SetCamera(m_camera);
			// Prepare shaders for each object (only testing with one model and shader atm)
			itr->second->Prepare(m_assimpShaderSource.VertexSource, m_assimpShaderSource.FragmentSource);
		}

		if (itr->first == "Taxi")
		{
			itr->second->SetCamera(m_camera);
			itr->second->Prepare(m_testShaderSource.VertexSource, m_testShaderSource.FragmentSource);
		}
	}
}

void GameWorld::Update()
{
	// Blue sky
	glClearColor(0.0, 0.0, 0.5, 1.0);

	// Render terrain
	m_terrain.Render();

	// Testing player
	m_player->Render();
		
	// Update all physics body locations *** All asset rendering is done through here for now because I dont want to have to call asset render twice ***
	UpdatePhysics();
}

void GameWorld::Destroy()
{
	// Delete all textures
	TextureManager::Instance().ReleaseAllTextures();

	std::multimap<std::string, IGameAsset*>::iterator itr;
	for (itr = m_gameAssets.begin(); itr != m_gameAssets.end(); itr++)
	{
		itr->second->Destroy();
	}
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

	float newY = m_terrain.GetAverageHeight(m_camera->GetPosition().x, m_camera->GetPosition().z) + 15;

	// Set updated camera location
	m_camera->SetPosition(glm::vec3(temp2.getX(), newY, temp2.getZ()));

	// Draw each object at the updated positions based on physics simulation
	std::multimap<std::string, IGameAsset*>::iterator itr;
	// Start at 1 because 0 is the cameras information
	int i = 1;
	for (itr = m_gameAssets.begin(); i < m_collisionBodyPos.size() || itr != m_gameAssets.end(); itr++)
	{
		glm::vec3 temp = glm::vec3(m_collisionBodyPos[i].x(), m_collisionBodyPos[i].y(), m_collisionBodyPos[i].z());

		if (itr->first == "Cube")
		{
			itr->second->SetPosition(temp);
			itr->second->Render();
		}

		if (itr->first == "Taxi")
		{
			itr->second->SetPosition(temp);
			itr->second->Render();
		}
		i++;
	}
}