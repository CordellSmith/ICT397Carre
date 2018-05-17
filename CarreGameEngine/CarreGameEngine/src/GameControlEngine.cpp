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


	if (!m_windowManager || m_windowManager->Initialize(ScreenWidth, ScreenHeight, screenTitle, fullScreen) != 0)
		exit(-1);

	glViewport(0, 0, ScreenWidth, ScreenHeight);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Initialize gameworld
	m_gameWorld = new GameWorld();
	glm::vec3 test;
	// Set camera perspective and position
	m_camera->SetPerspective(glm::radians(camFOV), ScreenWidth / (float)ScreenHeight, camNearPlane, camFarPlane);
	m_camera->PositionCamera(camPos.x, camPos.y, camPos.z, camYaw, camPitch);
	
	// Pass camera into gameworld
	m_gameWorld->SetCamera(m_camera);

	// Initialize asset factory
	m_assetFactory = new GameAssetFactory();

	// Initialize physics engine
	m_physicsWorld = new PhysicsEngine();

	ShaderSource testShader = ParseShaders("res/shaders/Test.shader");
	ShaderSource assimpShader = ParseShaders("res/shaders/Default.shader");

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
	bfTerrain.LoadHeightfield("res/terrain/city.raw", 128);
	bfTerrain.AddShader(testShader.VertexSource, testShader.FragmentSource);
	bfTerrain.GenerateTerrain(TextureManager::Instance().LoadTexture("res/terrain/grass.jpg"), "res/terrain/grass.jpg");
	bfTerrain.SetPosition(glm::vec3(0.0, 0.0, 0.0));

	// Everything has been scaled up by 15 because the terrain scaleX and scaleZ is at 15 per triangle grid
	// Cube asset
	IGameAsset* cube = m_assetFactory->CreateAsset(ASS_OBJECT, "Cube");
	cube->LoadFromFilePath("res/objects/cube.obj");
	cube->Prepare(assimpShader.VertexSource, assimpShader.FragmentSource);
	cube->SetPosition(glm::vec3(200.0, bfTerrain.GetHeight(200, 250) + 15, 250.0));
	cube->SetScale(glm::vec3(15.0, 15.0, 15.0));
	m_assetFactory->AddAsset(cube);

	// Taxi asset
	//IGameAsset* taxi = m_assetFactory->CreateAsset(ASS_OBJECT, "Taxi");
	//taxi->LoadFromFilePath("res/objects/taxi/taxi.obj");
	//taxi->Prepare(testShader.VertexSource, testShader.FragmentSource);
	//taxi->SetPosition(glm::vec3(200.0, bfTerrain.GetHeight(200, 200) + 15, 200.0));
	//taxi->SetScale(glm::vec3(15.0, 15.0, 15.0));
	//taxi->SetRotation(glm::vec3(0.0, 15.0, 15.0));
	//m_assetFactory->AddAsset(taxi);

	// Main character creation
	player = new Player("Player");
	player->LoadFromFilePath("res/objects/taxi/taxi.obj");
	player->Prepare(testShader.VertexSource, testShader.FragmentSource);
	player->SetPosition(glm::vec3(700.0, bfTerrain.GetHeight(700.0, 600.0) + 15, 600.0));
	player->SetScale(glm::vec3(15.0, 15.0, 15.0));

	m_windowManager->GetInputManager()->SetPlayer(player);
	m_camera->PassPlayerInfo(player->GetPosition(), player->GetRotation());

	// Pass the main character / player to the camera for 3rd person PoV
	//m_camera->SetPlayer(taxi);

	// Physics engine initialization
	InitializePhysics();

	// Initialize the game world, pass in terrain, assets and physics engine *** Can be reworked *** 
	m_gameWorld->SetTerrain(bfTerrain);
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
	glm::vec3 windowPerspective(m_camera->GetPosition());
	btVector3 cameraPosition(windowPerspective.x, windowPerspective.y, windowPerspective.z);
	m_physicsWorld->CreatePlayerControlledRigidBody(cameraPosition);
	m_collisionBodyPos.push_back(cameraPosition);

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
