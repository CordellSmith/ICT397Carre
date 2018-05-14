#include "..\headers\GameWorld.h"

void GameWorld::Init(std::multimap<ASS_TYPE, IGameAsset*> gameAssets)
{
	// Sets this game contexts assets to the  loaded game assets from the control engine
	SetGameAssets(gameAssets);

	m_terrain.SetCamera(m_camera);

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

}

void GameWorld::Update()
{
	// Blue sky
	glClearColor(0.0, 0.0, 0.5, 1.0);

	// Used for the camara to follow terrain
	float newY = m_terrain.GetAverageHeight(m_camera->GetPosition().x, m_camera->GetPosition().z) + 15;
	m_camera->SetPosition(glm::vec3(m_camera->GetPosition().x, newY, m_camera->GetPosition().z));

	//std::cout << "X: " << m_camera->GetPosition().x << " Y: " << m_terrain.GetAverageHeight(m_camera->GetPosition().x, m_camera->GetPosition().z) << " Z: " << m_camera->GetPosition().z << std::endl;

	// Update all physics body locations
	//UpdatePhysics();

	m_terrain.Render();

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