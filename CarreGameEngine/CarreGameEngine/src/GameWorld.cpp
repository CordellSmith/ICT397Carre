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

void GameWorld::Init()
{
	PrepareColourPanel();
	m_colourPanel.Initialize(panel, 6, m_shaderSource2.VertexSource, m_shaderSource2.FragmentSource);

	PrepareTestModel("res/objects/taxi_model/taxi.obj", m_modelVertexSize);
	m_testModel.Initialize(model, m_modelVertexSize, m_shaderSource1.VertexSource, m_shaderSource1.FragmentSource);
}

void GameWorld::Update()
{
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
	if (err = fopen_s(&file, filePath, "r") != 0)
	{
		if (file == NULL) {
			printf("Impossible to open the file !\n");
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

void GameWorld::PrepareColourPanel()
{
	m_shaderSource2 = ParseShader("res/shaders/Basic.shader");
	m_colourPanel.SetCamera(m_camera);

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
