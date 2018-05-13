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
	// Must initialise shader
	NewModel() { m_shader = new Shader(); }
	~NewModel() { Destroy(); }

	void LoadModel(std::string filePath);

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	void Prepare(std::string vertShader, std::string fragShader);
	void Draw();
	void Destroy();

	glm::vec3 GetPosition() { return m_position; }
	void SetPosition(glm::vec3 position);

	glm::vec3 GetRotation() { return m_rotation; }
	void SetRotation(glm::vec3 rotation) { m_rotation = rotation; }

	glm::vec3 GetScale() { return m_scale; }
	void SetScale(glm::vec3 scale);

		/**
		* @brief Sets the camera object
		*
		* Sets the camera object to the world camera object to retrieve the view and projection
		* matrices.
		*
		* @param Camera* camera
		* @return void
		*/
	const void SetCamera(Camera* camera);

	std::vector<Mesh>& GetMeshBatch() { return m_meshBatch; }
	std::vector<Texture>& GetTextures() { return m_texturesLoaded; }

protected:
	std::vector<Mesh> m_meshBatch;
	std::vector<Texture> m_texturesLoaded;
	std::string m_directory;

	Shader* m_shader;

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
};
