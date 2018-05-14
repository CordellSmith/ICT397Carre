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
	if (!m_windowManager || m_windowManager->Initialize(ScreenWidth, ScreenHeight, "Carre Game Engine", false) != 0)
		exit(-1);

	glViewport(0, 0, ScreenWidth, ScreenHeight);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Set camera perspective and position
	m_camera->SetPerspective(glm::radians(60.0f), ScreenWidth / (float)ScreenHeight, 0.01f, 1000);
	m_camera->PositionCamera(200, 2, 200, 0, 0);
	
	// Pass camera into gameworld
	m_gameWorld.SetCamera(m_camera);

	m_assetFactory = new GameAssetFactory();

	//IGameAsset* light = m_assetFactory->CreateAsset(ASS_OBJECT, "TrafficLight");
	//light->LoadFromFilePath("res/objects/trafficlight/trafficlight.obj");
	//light->Prepare(testShader.VertexSource, testShader.FragmentSource);
	//light->SetAssetPosition(glm::vec3(3.0, -1.0, 1.0));
	//light->SetAssetScale(glm::vec3(0.2, 0.2, 0.2));
	//m_assetFactory->AddAsset(light);

	//IGameAsset* terrain = m_assetFactory->CreateAsset(ASS_TERRAIN, "Terrain");
	//terrain->LoadFromFilePath("res/terrain/terraininfo.txt");

	bfTerrain.LoadHeightfield("res/terrain/height128.raw", 128);
	ShaderSource testShader = ParseShaders("res/shaders/Test.shader");
	bfTerrain.AddShader(testShader.VertexSource, testShader.FragmentSource);
	bfTerrain.GenerateTerrain(TextureManager::Instance().LoadTexture("res/terrain/grass.jpg"), "res/terrain/grass.jpg");
	bfTerrain.SetPosition(glm::vec3(0.0, 0.0, 0.0));

	// Everything has been scaled up by 15 because the terrain scaleX and scaleZ is at 15 per triangle grid
	IGameAsset* cube = m_assetFactory->CreateAsset(ASS_OBJECT, "Cube");
	ShaderSource assimpShader = ParseShaders("res/shaders/Default.shader");
	cube->LoadFromFilePath("res/objects/cube.obj");
	cube->Prepare(assimpShader.VertexSource, assimpShader.FragmentSource);
	cube->SetAssetPosition(glm::vec3(200.0, bfTerrain.GetHeight(200, 250) + 15, 250.0));
	cube->SetAssetScale(glm::vec3(15.0, 15.0, 15.0));
	m_assetFactory->AddAsset(cube);

	IGameAsset* taxi = m_assetFactory->CreateAsset(ASS_OBJECT, "Taxi");
	taxi->LoadFromFilePath("res/objects/taxi/taxi.obj");
	taxi->Prepare(testShader.VertexSource, testShader.FragmentSource);
	taxi->SetAssetPosition(glm::vec3(200.0, bfTerrain.GetHeight(200, 200) + 15, 200.0));
	taxi->SetAssetScale(glm::vec3(15.0, 15.0, 15.0));
	m_assetFactory->AddAsset(taxi);

	// Initialize the game world, pass in assets
	m_gameWorld.SetTerrain(bfTerrain);
	m_gameWorld.Init(m_assetFactory->GetAssets());
}

void GameControlEngine::GameLoop()
{
	while (m_windowManager->ProcessInput(true))
	{
		// Use our TimeManager singleton to calculate our framerate every frame
		TimeManager::Instance().CalculateFrameRate(true);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Update the game world
		m_gameWorld.Update();

		// Swap buffers
		m_windowManager->SwapTheBuffers();
	}
}

void GameControlEngine::Destroy()
{
	m_gameWorld.Destroy();

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
