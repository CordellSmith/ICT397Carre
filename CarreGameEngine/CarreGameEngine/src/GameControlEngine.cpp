#include "..\headers\GameControlEngine.h"

#include "GL/glew.h"

const int GameControlEngine::RunEngine()
{
	Initialize();

	GameLoop();

	Destroy();

	return 0;
}

void GameControlEngine::Initialize()
{
	// Initialize from script
	ScriptManager::Instance().LoadWindowInitLua(ScreenWidth, ScreenHeight, screenTitle, fullScreen);
	ScriptManager::Instance().LoadCamInitLua(camPos, camYaw, camPitch, camFOV, camNearPlane, camFarPlane);
	ScriptManager::Instance().LoadModelsInitLua(m_allModelsData, m_modelsData);
	ScriptManager::Instance().LoadHeightmapsInitLua(m_allHeightmapsData, m_heightmapsData);

	if (!m_windowManager || m_windowManager->Initialize(ScreenWidth, ScreenHeight, screenTitle, fullScreen) != 0)
		exit(-1);

	glViewport(0, 0, ScreenWidth, ScreenHeight);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Initialize gameworld
	m_gameWorld = new GameWorld();

	// Holds terrains
	std::vector<Bruteforce*> terrains;

	// Main landscape terrain
	Bruteforce* bfLandscape = new Bruteforce(100, 5, 100);
	// Building terrain
	Bruteforce* bfBuildings = new Bruteforce(100, 4, 100);

	// Create new player
	Player* player = new Player("Player");

	// Set camera perspective and position
	m_camera->SetPerspective(glm::radians(camFOV), ScreenWidth / (float)ScreenHeight, camNearPlane, camFarPlane);
	m_camera->PositionCamera(camPos.x, camPos.y, camPos.z, camYaw, glm::radians(camPitch));
	m_camera->SetPosition(glm::vec3(700, bfLandscape->GetAverageHeight(700, 700), 700.0));
		
	// Pass camera into gameworld
	m_gameWorld->SetCamera(m_camera);

	// Initialize asset factory
	m_assetFactory = new GameAssetFactory();

	// Initialize physics engine
	m_physicsWorld = new PhysicsEngine();

	// Load all textures
	//ScriptManager::Instance().LoadTexturesInitLua();

	// Light asset
	/*
	IGameAsset* light = m_assetFactory->CreateAsset(ASS_OBJECT, "TrafficLight");
	light->LoadFromFilePath("res/objects/trafficlight/trafficlight.obj");
	light->Prepare(testShader.VertexSource, testShader.FragmentSource);
	light->SetPosition(glm::vec3(3.0, -1.0, 1.0));
	light->SetScale(glm::vec3(0.2, 0.2, 0.2));
	m_assetFactory->AddAsset(light);
	*/

	/*
		When creating .raw files in Gimp. Make sure the file is Grey-scale when creating and when exporting, 
		make sure to select raw image format (.data) and Planar (RRR GGG BBB). Then you can just rename the .data 
		extension to .raw and its all good to go!
	*/

	// Bruteforce terrain
	bfLandscape->LoadHeightfield("res/terrain/newcity.raw", 128);
	bfLandscape->GenerateTerrain(TextureManager::Instance().LoadTexture("res/terrain/grass.jpg"), "res/terrain/grass.jpg");
	bfLandscape->SetPosition(glm::vec3(0.0, 0.0, 0.0));
	terrains.push_back(bfLandscape);

	bfBuildings->LoadHeightfield("res/terrain/buildingheightmap.raw", 16);
	bfBuildings->GenerateTerrain(TextureManager::Instance().LoadTexture("res/terrain/buildingtexture.jpg"), "res/terrain/buildingtexture.jpg");
	bfBuildings->SetPosition(glm::vec3(6000.0, -1.0, 5000.0));
	terrains.push_back(bfBuildings);

	IGameAsset* md2Model = m_assetFactory->CreateAsset(ASS_OBJECT, "md2");
	md2Model->LoadFromFilePath("res/objects/knight.md2");
	md2Model->AddTexutre(TextureManager::Instance().LoadTexture("res/objects/knight.bmp"), "res/objects/knight.bmp");
	md2Model->SetPosition(glm::vec3(10.0, 50.0, 10.0));
	md2Model->SetScale(glm::vec3(10.0, 10.0, 10.0));
	m_assetFactory->AddAsset(md2Model);
	



	/********************************************TESTING********************************************/
	// Get iterator to start of map
	std::unordered_map<std::string, ModelsData>::iterator it = m_allModelsData.begin();

	// Search map for texture
	while (it != m_allModelsData.end())
	{
		if ((*it).first == "player")
		{
			m_modelsData = (*it).second;
			break;
		}
		// Increment iterator
		it++;
	}	

	// Main character creation (filePath and scale read from m_modelsData)
	player->LoadFromFilePath(m_modelsData.filePath);
	player->SetPosition(glm::vec3(m_modelsData.modelPositions[0][0], m_modelsData.modelPositions[0][1], m_modelsData.modelPositions[0][2]));
	player->SetScale(glm::vec3(m_modelsData.modelScales[0][0], m_modelsData.modelScales[0][1], m_modelsData.modelScales[0][2]));

	/********************Loading of all models at once example*******************/
	// Create asset
	IGameAsset* modelAsset;

	// Temp values for naming of assets
	std::string tempName = "Asset";
	std::string assetName;
	
	// Asset xyz scale and pos
	float assetScaleXYZ[3];
	float assetPosXYZ[3];

	// Start of map again
	it = m_allModelsData.begin();

	// Loop through map until all models created
	while (it != m_allModelsData.end())
	{
		// For each different type of model (player already read in, so skip)
		if ((*it).first != "player")
		{
			// For each model of same type
			for (int k = 0; k < (*it).second.modelPositions.size(); k++)
			{
				// Get scales
				for (int j = 0; j < (*it).second.modelScales[k].size(); j++)
				{
					assetScaleXYZ[j] = (*it).second.modelScales[k][j];
				}

				// Get positions
				for (int j = 0; j < (*it).second.modelPositions[k].size(); j++)
				{
					assetPosXYZ[j] = (*it).second.modelPositions[k][j];
				}

				// Create name asset data and add to asset map
				modelAsset = m_assetFactory->CreateAsset(ASS_OBJECT, (*it).first);
				modelAsset->LoadFromFilePath((*it).second.filePath);
				//modelAsset->AddTexutre(TextureManager::Instance().LoadTexture("res/terrain/grass.jpg"), "res/terrain/grass.jpg");
				modelAsset->AddTexutre(0, "");
				modelAsset->SetScale(glm::vec3(assetScaleXYZ[0], assetScaleXYZ[1], assetScaleXYZ[2]));
				modelAsset->SetPosition(glm::vec3(assetPosXYZ[0], assetPosXYZ[1], assetPosXYZ[2]));
				m_assetFactory->AddAsset(modelAsset);
			}
		}
		// Increment iterator
		it++;
	}
	/********************Loading of all models at once example*******************/

	/********************************************TESTING********************************************/
	




	m_windowManager->GetInputManager()->SetPlayer(player);

	// Physics engine initialization
	InitializePhysics();

	// Initialize the game world, pass in terrain, assets and physics engine *** Can be reworked *** 
	m_gameWorld->SetTerrains(terrains);
	m_gameWorld->Init(player, m_assetFactory->GetAssets());
	m_gameWorld->SetPhysicsWorld(m_physicsWorld, m_collisionBodyPos);
}

void GameControlEngine::GameLoop()
{
	while (m_windowManager->ProcessInput(true))
	{
		// Use our TimeManager singleton to calculate our framerate every frame
		TimeManager::Instance().CalculateFrameRate(true);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Update the game world
		m_gameWorld->Update();

		// Swap buffers
		m_windowManager->SwapTheBuffers();
	}
}

void GameControlEngine::InitializePhysics()
{
	// Create camera rigid body to collide with objects
	glm::vec3 camerPos(m_camera->GetPosition());
	btVector3 bt_cameraPos(camerPos.x, camerPos.y, camerPos.z);
	m_physicsWorld->CreatePlayerControlledRigidBody(bt_cameraPos);
	m_collisionBodyPos.push_back(bt_cameraPos);

	// Add player (taxi) to the rigid bodies
	glm::vec3 playerPos(m_camera->GetPosition());
	btVector3 bt_playerPos(playerPos.x, playerPos.y, playerPos.z);
	m_collisionBodyPos.push_back(bt_playerPos);

	// Loop through map and add all assets to the collision body list
	std::multimap<std::string, IGameAsset*>::const_iterator itr;
	for (itr = m_assetFactory->GetAssets().begin(); itr != m_assetFactory->GetAssets().end(); itr++)
	{
		m_physicsWorld->CreateStaticRigidBody();
		m_collisionBodyPos.push_back(btVector3(itr->second->GetPosition().x, 
			itr->second->GetPosition().y, itr->second->GetPosition().z));
	}

	//  *** Can this be changed to the terrain mesh? *** 
	// Create static rigid body (floor)
	//m_physicsWorld->CreateStaticRigidBody();
	//m_collisionBodyPos->push_back(btVector3(0.0, 0.0, 0.0));

	// Activate all rigid body objects
	m_physicsWorld->ActivateAllObjects();
}

void GameControlEngine::Destroy()
{
	m_gameWorld->Destroy();

	/// Delete all textures
	//TextureManager::Instance().~TextureManager();

	/// Delete window
	if (m_windowManager)
	{
		m_windowManager->Destroy();

		delete m_windowManager;
		m_windowManager = nullptr;
	}

	/// Delete camera
	if (m_camera)
	{
		delete m_camera;
		m_camera = nullptr;
	}
}
