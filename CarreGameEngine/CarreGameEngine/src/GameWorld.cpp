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

	m_camera->PassPlayerInfo(m_player->GetPosition(), m_player->GetRotation());
	
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
	}
}

void GameWorld::Update()
{
	// Blue sky
	glClearColor(0.0, 0.0, 0.5, 1.0);

	// Render terrain
	m_terrain.Render();

	// Testing player
	m_player->SetPosition(glm::vec3(
		m_player->GetPosition().x,
		m_terrain.GetAverageHeight(m_player->GetPosition().x, m_player->GetPosition().z),
		m_player->GetPosition().z
	));
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

	float newY = m_terrain.GetAverageHeight(m_camera->GetPosition().x, m_camera->GetPosition().z) + 50;

	// Set updated camera location
	m_camera->SetPosition(glm::vec3(m_player->GetPosition().x, newY, m_player->GetPosition().z));
		
	// Draw each object at the updated positions based on physics simulation
	std::multimap<std::string, IGameAsset*>::iterator itr;
	int i = 1;
	for (itr = m_gameAssets.begin(); itr != m_gameAssets.end(); itr++)
	{
		glm::vec3 temp = glm::vec3(m_collisionBodyPos[i].x(), m_terrain.GetAverageHeight(m_collisionBodyPos[i].x(), m_collisionBodyPos[i].z()), m_collisionBodyPos[i].z());

		if (itr->first == "Cube")
		{
			itr->second->SetPosition(temp);
			itr->second->Render();
		}
		i++;
	}
}