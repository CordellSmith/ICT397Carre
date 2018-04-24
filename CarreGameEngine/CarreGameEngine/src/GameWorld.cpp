#include "..\headers\GameWorld.h"

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

	return { ss[0].str(), ss[1].str() };
}

Vertex panel[6] = { glm::vec3(0), glm::vec4(1), glm::vec2(2), glm::vec3(3) };
Vertex model[49971] = { glm::vec3(0), glm::vec4(1), glm::vec2(2), glm::vec3(3) };
std::vector<unsigned int> m_indices;

void GameWorld::Init(std::multimap<OBJ_TYPE, IGameObject*> gameAssets)
{

	/* TODO
	Call render on all game assets using multimap iterator
	Something like
	std::multimap::iterator itr
	for (itr = multimap.begin(); itr != multimap.end(); itr++)
	{
		// Access the second item in pair which is the asset
		// Prepare function should create the link between the models VAO, VBO (and EBO if used)
		itr->second.Prepare();
		
		// Rendering occurs in the update function
	}
	*/

	PrepareColourPanel();
	m_colourPanel.Initialize(panel, 6, m_indices, m_shaderSource2.VertexSource, m_shaderSource2.FragmentSource);

	PrepareTestModel("res/objects/taxi_model/taxi.obj", m_modelVertexSize);
	m_testModel.Initialize(model, m_modelVertexSize, m_indices, m_shaderSource1.VertexSource, m_shaderSource1.FragmentSource);

	// Initialize all physics objects
	InitializePhysics();
}

void GameWorld::Update()
{
	// Update all physics body locations
	UpdatePhysics();

	// Render a floor on x/z axis (15 x 15)
	for (int x = 0; x < 15; x++)
	{
		for (int z = 0; z < 15; z++)
		{
			m_colourPanel.SetPosition(glm::vec3(x * 2, 0, z * 2));
			m_colourPanel.Render();
		}
	}
}

void GameWorld::Destroy()
{
	m_colourPanel.Destroy();
	m_testModel.Destroy();
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

	// Draw shapes for testing (just planes atm, didn't know how to make spheres using current setup)
	//glm::vec3 temp = glm::vec3(m_collisionBodyPos[0].x(), m_collisionBodyPos[0].y(), m_collisionBodyPos[0].z());
	//colourPanel.SetPosition(glm::vec3(temp.x, temp.y, temp.z));

	// Draw each object at the updated positions based on physics simulation
	for (int i = 0; i < m_collisionBodyPos.size(); i++)
	{
		glm::vec3 temp = glm::vec3(m_collisionBodyPos[i].x(), m_collisionBodyPos[i].y(), m_collisionBodyPos[i].z());

		// First collision body is camera, second is the the static ground plane, so draw a colour panel instead of a car
		if (i == 0 || i == 1)
		{
			m_colourPanel.SetPosition(glm::vec3(temp.x, temp.y, temp.z));
			m_colourPanel.Render();
			continue;
		}
		m_testModel.SetPosition(glm::vec3(temp.x, temp.y, temp.z));
		m_testModel.Render();
	}
}

void GameWorld::PrepareTestModel(const char* filePath, int& modelVertexSize)
{
	m_shaderSource1 = ParseShader("res/shaders/Cube.shader");
	m_testModel.SetCamera(m_camera);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	errno_t err;

	FILE* file;
	if (err = fopen_s(&file, filePath, "r") == 0)
	{
		if (file == NULL) {
			std::cout << "Impossible to open the file !\n" << std::endl;
		}
		while (1) {
			char lineHeader[128];
			// read the first word of the line
			int res = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader));
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.

			if (strcmp(lineHeader, "v") == 0) {
				glm::vec3 vertex;
				fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z, sizeof(&vertex.x), sizeof(&vertex.y), sizeof(&vertex.z));
				temp_vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0) {
				glm::vec2 uv;
				fscanf_s(file, "%f %f\n", &uv.x, &uv.y, sizeof(&uv.x), sizeof(&uv.y));
				temp_uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0) {
				glm::vec3 normal;
				fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z, sizeof(&normal.x), sizeof(&normal.y), sizeof(&normal.z));
				temp_normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2],
					sizeof(&vertexIndex[0]), sizeof(&uvIndex[0]), sizeof(&normalIndex[0]), sizeof(&vertexIndex[0]), sizeof(&uvIndex[0]), sizeof(&normalIndex[0]), sizeof(&vertexIndex[0]), sizeof(&uvIndex[0]), sizeof(&normalIndex[0]));
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
	}

	// Populate temporary attribute vectors
	float red = 0.1f, blue = 0.5f, green = 0.4f;
	for (unsigned int i = 0; i < temp_vertices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];

		model[i].xyz = vertex;
		model[i].rgba = glm::vec4(red, green, blue, 1.0);
		m_indices.push_back(vertexIndices[i]);
	}

	for (unsigned int i = 0; i < temp_uvs.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];

		model[i].uv = uv;
	}

	for (unsigned int i = 0; i < temp_normals.size(); i++) {
		unsigned int normalsIndex = normalIndices[i];
		glm::vec3 normals = temp_normals[normalsIndex - 1];

		model[i].normal = normals;
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalsIndex = normalIndices[i];

		m_indices.push_back(vertexIndices[i]);
		m_indices.push_back(vertexIndices[i]);
		m_indices.push_back(vertexIndices[i]);

	}

	modelVertexSize = temp_vertices.size();
	m_testModel.SetScale(glm::vec3(0.75, 0.75, 0.75));
}

void GameWorld::PrepareColourPanel()
{
	m_shaderSource2 = ParseShader("res/shaders/Basic.shader");
	m_colourPanel.SetCamera(m_camera);

	panel[0].xyz = glm::vec3(-1.0f, -1.0f, -1.0f);
	panel[0].rgba = glm::vec4(0.5, 0.5, 0.0, 1);

	panel[1].xyz = glm::vec3(1.0f, -1.0f, -1.0f);
	panel[1].rgba = glm::vec4(0.4, 0.0, 1.0, 1);

	panel[2].xyz = glm::vec3(1.0f, -1.0f, 1.0f);
	panel[2].rgba = glm::vec4(0.5, 0.6, 1.0, 1);

	panel[3].xyz = glm::vec3(1.0f, -1.0f, 1.0f);
	panel[3].rgba = glm::vec4(1.0, 0.4, 0.4, 1);

	panel[4].xyz = glm::vec3(-1.0f, -1.0f, 1.0f);
	panel[4].rgba = glm::vec4(0.3, 1.0, 0.1, 1);

	panel[5].xyz = glm::vec3(-1.0f, -1.0f, -1.0f);
	panel[5].rgba = glm::vec4(1.0, 1.0, 0.4, 1);

	for (int i = 1; i < 7; i++)
		m_indices.push_back(i);
}
