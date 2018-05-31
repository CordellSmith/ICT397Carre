#include "..\headers\GameWorld.h"

// Animation testing
#include "..\Animation\DynamicObject.h"

DynamicObject* md2file;
double lastTime;
int randomPositions[20];
int randomRotations[20];

void GameWorld::Init(Player* player, std::multimap<std::string, IGameAsset*> gameAssets)
{
	// Sets this game contexts assets to the  loaded game assets from the control engine
	SetGameAssets(gameAssets);

	// Each new entity, asset needs its own shader (terrain or anything with multiple meshes especially)
	ShaderSource mainShader = ParseShaders("res/shaders/Default.shader");
	ShaderSource terrainShader = ParseShaders("res/shaders/Terrain.shader");

	// Prepare player
	m_player = player;
	m_player->SetCamera(m_camera);
	m_glRenderer.Prepare(m_player->GetModel(), mainShader.VertexSource, mainShader.FragmentSource);

	// Pass player info to camera
	m_camera->ParsePlayerInfo(m_player->GetPosition(), m_player->GetRotation());
	float horizontalDistance = m_camera->CalculateHorizontalDistance();
	float verticalDistance = m_camera->CalculateVerticalDistance();
	m_camera->CalculateCameraPosition(horizontalDistance, verticalDistance);

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
		itr->second->SetCamera(m_camera);
		m_glRenderer.Prepare(itr->second->GetModel(), mainShader.VertexSource, mainShader.FragmentSource);
	
		if (itr->first == "md2")
		{
			itr->second->SetCamera(m_camera);
			m_glRenderer.Prepare(itr->second->GetModel(), mainShader.VertexSource, mainShader.FragmentSource);
		}
	}

	// AI testing
	/*ComputerAI p;
	for (int i = 0; i < 1000; i++)
		p.Update();
	getchar();*/

	// Animation testing
	//md2file = new DynamicObject(2.5, 512, 512, 0, 100);
	//md2file->Import("res/objects/knight.md2", glm::mat4());

	for (int i = 0; i < 15; i++)
	{
		int randNum1 = rand() % (6000 - 0 + 1) + 0;
		int randNum2 = rand() % (360 - 1 + 1) + 1;

		randomPositions[i] = randNum1;
		randomRotations[i] = randNum2;
	}
}

void GameWorld::Update()
{
	// Camera
	m_camera->ParsePlayerInfo(m_player->GetPosition(), m_player->GetRotation());
	float horizontalDistance = m_camera->CalculateHorizontalDistance();
	float verticalDistance = m_camera->CalculateVerticalDistance();
	m_camera->CalculateCameraPosition(horizontalDistance, verticalDistance);

	//glm::vec3 test = m_player->GetPosition();
	//std::cout << test.x << " " << test.y << " " << test.z << std::endl;

	// Blue sky
	glClearColor(0.0, 0.0, 0.5, 1.0);

	// Render terrain
	for each (Bruteforce* terrain in m_terrains)
	{
		m_glRenderer.Render(terrain->GetModel());
	}

	// Render player
	m_glRenderer.Render(m_player->GetModel());

	// Update all physics body locations *** All asset rendering is done through here for now because I dont want to have to call asset render twice ***
	UpdatePhysics();

	// Animation
	//double now = glfwGetTime();
	//md2file->Animate(now - lastTime);
	//lastTime = now;

	//md2file->UpdateVAO();

	//md2file->Draw();
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

// Update all physics
void GameWorld::UpdatePhysics()
{
	// Update physicsWorld
	// TODO: Make this better (Jack)
	glm::vec3 temp1(m_player->GetPosition());
	
	btVector3 temp2(temp1.x, temp1.y, temp1.z);
	m_physicsWorld->Simulate(m_collisionBodyPos, temp2);

	// Set updated camera location
	m_camera->SetPosition(glm::vec3(m_camera->GetPosition().x, m_camera->GetPosition().y, m_camera->GetPosition().z));
		
	// Draw each object at the updated positions based on physics simulation
	std::multimap<std::string, IGameAsset*>::iterator itr;
	int i = 1;
	ComputerAI* compAI;
	for (itr = m_gameAssets.begin(); itr != m_gameAssets.end(); itr++)
	{
		glm::vec3 temp = glm::vec3(m_collisionBodyPos[i].x(), m_terrains[0]->GetAverageHeight(m_collisionBodyPos[i].x(), m_collisionBodyPos[i].z()) + 100, m_collisionBodyPos[i].z());
		//glm::vec3 temp = glm::vec3(m_collisionBodyPos[i].x(), m_collisionBodyPos[i].y(), m_collisionBodyPos[i].z());

		compAI = itr->second->GetAI();
		if (compAI != NULL)
		{
			compAI->Update();

			Vector2 tempPos = compAI->GetPosition();
			itr->second->SetPosition(glm::vec3(tempPos.x, temp.y, tempPos.z));
		}
		else
		{
			itr->second->SetPosition(temp);
		}

		if (itr->first == "knight")
		{
			/*compAI = itr->second->GetAI();
			compAI->Update();

			Vector2 tempPos = compAI->GetPosition();
			itr->second->SetPosition(glm::vec3(tempPos.x, temp.y, tempPos.z));*/

			for (int j = 0; j < 15; j++)
			{
				itr->second->SetPosition(glm::vec3(m_collisionBodyPos[i].x(), m_terrains[0]->GetAverageHeight(m_collisionBodyPos[i].x(), m_collisionBodyPos[i].z()), m_collisionBodyPos[i].z()));
				m_glRenderer.Render(itr->second->GetModel());
				i++;
			}
		}

		if (itr->first == "rock")
		{
			for (int j = 0; j < 5; j++)
			{
				itr->second->SetPosition(glm::vec3(m_collisionBodyPos[i].x(), m_terrains[0]->GetAverageHeight(m_collisionBodyPos[i].x(), m_collisionBodyPos[i].z()) - 20, m_collisionBodyPos[i].z()));
				m_glRenderer.Render(itr->second->GetModel());
				i++;
			}
		}
	}

	glm::vec3 tempPlayer = m_player->GetPosition();
	//m_player->SetPosition(glm::vec3(tempPlayer.x, m_terrains[0]->GetAverageHeight(tempPlayer.x, tempPlayer.z), tempPlayer.z));
	m_player->SetPosition(glm::vec3(temp2.getX(), m_terrains[0]->GetAverageHeight(temp2.getX(), temp2.getZ()), temp2.getZ()));
}