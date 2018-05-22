#include "..\headers\GameControlEngine.h"

#include "GL/glew.h"

/// Function to read a shader in one string and return a struct containing separated Vertex and Fragment shaders
ShaderSource ParseShaders(const std::string& filePath)
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

	return{ ss[0].str(), ss[1].str() };
}

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
	ScriptManager::Instance().LoadModelsInitLua();

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

	// Player
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

	ShaderSource testShader = ParseShaders("res/shaders/Test.shader");
	ShaderSource assimpShader = ParseShaders("res/shaders/Default.shader");
	ShaderSource terrainShader = ParseShaders("res/shaders/Terrain.shader");

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
	bfLandscape->AddShader(terrainShader.VertexSource, terrainShader.FragmentSource);
	bfLandscape->GenerateTerrain(TextureManager::Instance().LoadTexture("res/terrain/grass.jpg"), "res/terrain/grass.jpg");
	bfLandscape->SetPosition(glm::vec3(0.0, 0.0, 0.0));
	terrains.push_back(bfLandscape);

	bfBuildings->LoadHeightfield("res/terrain/buildingheightmap.raw", 16);
	bfBuildings->AddShader(terrainShader.VertexSource, terrainShader.FragmentSource);
	bfBuildings->GenerateTerrain(TextureManager::Instance().LoadTexture("res/terrain/buildingtexture.jpg"), "res/terrain/buildingtexture.jpg");
	bfBuildings->SetPosition(glm::vec3(6000.0, -1.0, 5000.0));
	terrains.push_back(bfBuildings);
	
	// Everything has been scaled up by 15 because the terrain scaleX and scaleZ is at 15 per triangle grid
	// Cube asset
	IGameAsset* cube = m_assetFactory->CreateAsset(ASS_OBJECT, "Cube");
	cube->LoadFromFilePath("res/objects/cube.obj");
	cube->Prepare(assimpShader.VertexSource, assimpShader.FragmentSource);
	cube->SetPosition(glm::vec3(200.0, bfLandscape->GetHeight(200, 250), 300.0));
	cube->SetScale(glm::vec3(15.0, 15.0, 15.0));
	m_assetFactory->AddAsset(cube);

	// Main character creation
	player->LoadFromFilePath("res/objects/taxi/taxi.obj");
	player->Prepare(testShader.VertexSource, testShader.FragmentSource);
	player->SetPosition(glm::vec3(m_camera->GetPosition().x, m_camera->GetPosition().y, m_camera->GetPosition().z));
	player->SetScale(glm::vec3(15.0, 15.0, 15.0));

	m_windowManager->GetInputManager()->SetPlayer(player);

	// Pass the main character / player to the camera for 3rd person PoV
	//m_camera->SetPlayer(taxi);

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
