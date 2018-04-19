#include "..\headers\GameControlEngine.h"

#include "GL/glew.h"
#include "..\headers\ScriptManager.h"

/// Struct to hold both vertex and fragment shaders
struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

/// Function to read a shader in one string and return a struct containing separated Vertex and Fragment shaders
ShaderSource ParseShader(const std::string& filePath)
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

Model colourPanel;
Model testModel;

Vertex panel[6] = { glm::vec3(0), glm::vec4(1), glm::vec2(2), glm::vec3(3) };
Vertex model[49971] = { glm::vec3(0), glm::vec4(1), glm::vec2(2), glm::vec3(3) };

void PrepareTestModel(const char* filePath, int& modelVertexSize)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE* file = fopen(filePath, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
	}

	while (1) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	std::vector<glm::vec3> v;
	std::vector<glm::vec2> u;
	std::vector<glm::vec3> n;

	float red = 0.1f, blue = 0.5f, green = 0.4f;
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		v.push_back(vertex);
	}

	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		u.push_back(uv);
	}

	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalsIndex = normalIndices[i];
		glm::vec3 normals = temp_normals[normalsIndex - 1];
		n.push_back(normals);
	}

	for (int i = 0; i < v.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];

		model[i].xyz = v[vertexIndex - 1];
		model[i].rgba = glm::vec4(red, green, blue, 1.0);
		model[i].uv = u[vertexIndex - 1];
		model[i].normal = n[vertexIndex - 1];
	}

	modelVertexSize = v.size();
}

void PreparePanel()
{
	panel[0].xyz = glm::vec3(-1.0f, -1.0f, -1.0f);
	panel[0].rgba = glm::vec4(1, 1, 0, 1);

	panel[1].xyz = glm::vec3(1.0f, -1.0f, -1.0f);
	panel[1].rgba = glm::vec4(1, 0, 0, 1);

	panel[2].xyz = glm::vec3(1.0f, -1.0f, 1.0f);
	panel[2].rgba = glm::vec4(0, 1, 1, 1);

	panel[3].xyz = glm::vec3(1.0f, -1.0f, 1.0f);
	panel[3].rgba = glm::vec4(0, 1, 1, 1);

	panel[4].xyz = glm::vec3(-1.0f, -1.0f, 1.0f);
	panel[4].rgba = glm::vec4(0, 0, 1, 1);

	panel[5].xyz = glm::vec3(-1.0f, -1.0f, -1.0f);
	panel[5].rgba = glm::vec4(1, 1, 0, 1);
}

const int GameControlEngine::RunEngine()
{
	Initialize();

	GameLoop();

	Destroy();

	return 0;
}

// This function initializes the window, the shaders and the triangle vertex data.
void GameControlEngine::Initialize()
{
	// Make sure the window manager is initialized prior to calling this and creates the OpenGL context
	if (!m_windowManager || m_windowManager->Initialize(ScreenWidth, ScreenHeight, "Carre Game Engine", false) != 0)
		exit(-1);

	glViewport(0, 0, ScreenWidth, ScreenHeight);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Testing script output
	loadScript();

	// Create shaders to use
	ShaderSource source = ParseShader("res/shaders/Basic.shader");
	ShaderSource cubeSource = ParseShader("res/shaders/Cube.shader");

	// Holds test models vertex size
	int modelVertexSize;

	// Prepare our models
	PreparePanel();
	PrepareTestModel("res/objects/taxi_model/taxi.obj", modelVertexSize);

	// Initialize our models
	colourPanel.Initialize(panel, 6, source.VertexSource, source.FragmentSource);
	testModel.Initialize(model, modelVertexSize, cubeSource.VertexSource, cubeSource.FragmentSource);

	// Set camera perspective and position
	m_camera->SetPerspective(glm::radians(60.0f), ScreenWidth / (float)ScreenHeight, 0.01f, 100);
	m_camera->PositionCamera(0, 0, 6, 0, 0);

	// Pass camera object to models
	colourPanel.SetCamera(m_camera);
	testModel.SetCamera(m_camera);

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

		// Render a floor on x/z axis (15 x 15)
		for (int x = 0; x < 15; x++)
		{
			for (int z = 0; z < 15; z++)
			{
				colourPanel.SetPosition(glm::vec3(x * 2, 0, z * 2));
				colourPanel.Render();
			}
		}

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
		colourPanel.SetPosition(glm::vec3(temp.x, temp.y, temp.z));

		for (int i = 1; i < collisionBodyPos.size(); i++)
		{
			//int a = collisionBodyPos[i].x;
			glm::vec3 temp = glm::vec3(collisionBodyPos[i].x(), collisionBodyPos[i].y(), collisionBodyPos[i].z());

			testModel.SetPosition(glm::vec3(temp.x, temp.y, temp.z));
			testModel.Render();
		}
		/**************************************************************************/
		m_windowManager->SwapTheBuffers();
	}
}

void GameControlEngine::Destroy()
{
	/// Delete models
	colourPanel.Destroy();
	testModel.Destroy();

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
