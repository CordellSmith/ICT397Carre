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
	if (!m_windowManager || m_windowManager->Initialize(ScreenWidth, ScreenHeight, "Carre Game Engine", false) != 0)
		exit(-1);

	glViewport(0, 0, ScreenWidth, ScreenHeight);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Testing script output
	loadScript();

	// Set camera perspective and position
	m_camera->SetPerspective(glm::radians(60.0f), ScreenWidth / (float)ScreenHeight, 0.01f, 100);
	m_camera->PositionCamera(0, 0, 6, 0, 0);
	
	// Pass camera into gameworld
	m_gameWorld.SetCamera(m_camera);

	// Initialize the game world
	m_gameWorld.Init();
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
