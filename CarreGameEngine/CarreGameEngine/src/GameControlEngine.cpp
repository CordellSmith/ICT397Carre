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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Testing script output
	loadScript();

	// Set camera perspective and position
	m_camera->SetPerspective(glm::radians(60.0f), ScreenWidth / (float)ScreenHeight, 0.01f, 100);
	m_camera->PositionCamera(0, 0, 6, 0, 0);
	
	// Pass camera into gameworld
	m_gameWorld.SetCamera(m_camera);
	m_gameWorld.Init();

	// Physics Testing
	// Create player object (camera)
	glm::vec3 tempCam(m_camera->GetPosition());
	btVector3 tempCam2(tempCam.x, tempCam.y, tempCam.z);
	physicsWorld.CreatePlayerControlledRigidBody(tempCam2);
	collisionBodyPos.push_back(tempCam2);
	// Create static rigid body (floor)
	physicsWorld.CreateStaticRigidBody();
	collisionBodyPos.push_back(btVector3(0.0, 0.0, 0.0));
	// Create dynamic rigid bodies
	physicsWorld.CreateDynamicRigidBody(btVector3(15.0, 15.0, 15.0));
	collisionBodyPos.push_back(btVector3(15.0, 15.0, 15.0));
	physicsWorld.CreateDynamicRigidBody(btVector3(15.0, 0.0, 15.0));
	collisionBodyPos.push_back(btVector3(15.0, 0.0, 15.0));
	// Heightmap terrain shape
	//physicsWorld.CreateHeightfieldTerrainShape();
	//collisionBodyPos.push_back(btVector3(0.0, 0.0, 0.0));
}

void GameControlEngine::GameLoop()
{
	while (m_windowManager->ProcessInput(true))
	{
		// Use our TimeManager singleton to calculate our framerate every frame
		TimeManager::Instance().CalculateFrameRate(true);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		m_gameWorld.Update();

		/**************************************************************************/
		// Update physicsWorld
		// TODO: Make this better (Jack)
		glm::vec3 temp1(m_camera->GetPosition());
		btVector3 temp2(temp1.x, temp1.y, temp1.z);
		physicsWorld.Simulate(collisionBodyPos, temp2);

		// Set updated camera location
		m_camera->SetPosition(glm::vec3(temp2.getX(), temp2.getY(), temp2.getZ()));

		// Draw shapes for testing (just planes atm, didn't know how to make spheres using current setup)
		glm::vec3 temp = glm::vec3(collisionBodyPos[0].x(), collisionBodyPos[0].y(), collisionBodyPos[0].z());
		//colourPanel.SetPosition(glm::vec3(temp.x, temp.y, temp.z));

		for (int i = 1; i < collisionBodyPos.size(); i++)
		{
			//int a = collisionBodyPos[i].x;
			glm::vec3 temp = glm::vec3(collisionBodyPos[i].x(), collisionBodyPos[i].y(), collisionBodyPos[i].z());

			//testModel.SetPosition(glm::vec3(temp.x, temp.y, temp.z));
			//testModel.Render();
		}
		/**************************************************************************/
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
