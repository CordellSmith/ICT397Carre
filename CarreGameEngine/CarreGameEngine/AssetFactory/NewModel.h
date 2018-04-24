#pragma once

#include <iostream>
#include <vector>
#include <GLM\glm.hpp>									// Used for the GLM math library
#include <GLM\gtc\matrix_transform.hpp>					
#include <GLM\gtx\transform2.hpp>						
#include <assimp/Importer.hpp>							// Assimp library used for loading
#include <assimp/scene.h>								// Guided by https://learnopengl.com/Model-Loading/Assimp
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "..\headers\Shader.h"

class NewModel
{
public:
	NewModel() { }
	~NewModel() { }

	void LoadModel(std::string filePath);

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

	void Draw();

	glm::vec3 GetPosition() { return m_position; }
	void SetPosition(glm::vec3 position) { m_position = position; }
	glm::vec3 GetRotation() { return m_rotation; }
	void SetRotation(glm::vec3 rotation) { m_rotation = rotation; }
	glm::vec3 GetScale() { return m_scale; }
	void SetScale(glm::vec3 scale) { m_scale = scale; }

protected:
	std::vector<Mesh> m_meshBatch;

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
};