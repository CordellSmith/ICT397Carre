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
#include "..\headers\Camera.h"
#include "..\headers\Shader.h"
#include "..\headers\TextureManager.h"
#include "..\headers\ComputerAI.h"

	/**
	* @class
	* @brief
	*
	* @author Cordell Smith
	* @version 01
	* @date 31/05/2018
	*
	*/
class Model
{
public:
	Model();
	~Model() { Destroy(); }

	void LoadModel(std::string filePath);

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	void Destroy();

	glm::vec3& GetPosition() { return m_position; }
	void SetPosition(glm::vec3 position);

	glm::vec3& GetRotation() { return m_rotation; }
	void SetRotation(glm::vec3 rotation);

	glm::vec3& GetScale() { return m_scale; }
	void SetScale(glm::vec3 scale);

	Camera* GetCamera() { return m_camera; }

		/**
		* @brief Sets the camera object
		*
		* Sets the camera object to the world camera object to retrieve the view and projection
		* matrices.
		*
		* @param Camera* camera
		* @return void
		*/
	const void SetCamera(Camera* camera) { m_camera = camera; }

	std::vector<Mesh>& GetMeshBatch() { return m_meshBatch; }
	std::vector<Texture>& GetTextures() { return m_texturesLoaded; }

	Shader* GetShader() { return m_shader; }
	void SetShader(Shader* shader) { m_shader = shader; }

	
	void SetAI(ComputerAI* compAI) { m_compAI = compAI; }
	ComputerAI* GetAI() { return m_compAI; }


	unsigned int VAO;
	
protected:
	std::vector<Mesh> m_meshBatch;
	std::vector<Texture> m_texturesLoaded;
	std::string m_directory;

	Shader* m_shader;
	Camera* m_camera;

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;


	ComputerAI* m_compAI;
};
