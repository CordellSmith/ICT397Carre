#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex3> vertices, int numOfVertexs, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	m_camera = new Camera();
	m_vertices = vertices;
	m_numOfVertexs = numOfVertexs;
	m_indices = indices;
	m_textures = textures;

	SetupMesh();
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * m_numOfVertexs, &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	// vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(m_vertices[0]), 0);
	// vertex colours
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(m_vertices[0]), (GLvoid*)sizeof(m_vertices[0].m_position));
	// vertex normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(m_vertices[0]), (GLvoid*)sizeof(m_vertices[0].m_colour));
	// vertex texture coords
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(m_vertices[0]), (GLvoid*)sizeof(m_vertices[0].m_normal));

	glBindVertexArray(0);
}

void Mesh::Draw(Shader* shader)
{
	shader->TurnOn();

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (size_t i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
										  // retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = m_textures[i].m_type;

		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);

		GLuint textureUniformId = shader->GetVariable(("material." + name + number).c_str());
		shader->SetFloat(textureUniformId, i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}
	glActiveTexture(GL_TEXTURE0);
	
	// Grab the view and projection matrices
	glm::mat4 projectionMatrix = m_camera->GetProjectionMatrix();
	glm::mat4 viewMatrix = m_camera->GetViewMatrix();

	// Order is important, must be Translate, Scale then Rotation
	// Create the Model Matrix with the current position
	glm::mat4 modelMatrix = glm::translate(glm::mat4(0.5f), m_position);

	// Similar to the Position above, we do the same for the m_scale
	modelMatrix = glm::scale(modelMatrix, m_scale);

	// HAVING ISSUES WITH MOUSE MOVEMENT, COMMENT OUT TO FIX
	// Add any rotation to the Model Matrix for each axis
	//modelMatrix = glm::rotate(modelMatrix, m_rotation.x, glm::vec3(1, 0, 0));
	//modelMatrix = glm::rotate(modelMatrix, m_rotation.y, glm::vec3(0, 1, 0));
	//modelMatrix = glm::rotate(modelMatrix, m_rotation.z, glm::vec3(0, 0, 1));

	// Get the uniform variables from the shader
	GLint modelMatrixId = shader->GetVariable("model");
	GLint viewMatrixId = shader->GetVariable("view");
	GLint projectionMatrixId = shader->GetVariable("projection");

	// Set the model, view and projection matrix for the uniform matrices in the Vertex Shader.
	// To get at a GLM glm::mat4 value, we use the address of the first item [0][0].
	shader->SetMatrix4(modelMatrixId, 1, false, &modelMatrix[0][0]);
	shader->SetMatrix4(viewMatrixId, 1, false, &viewMatrix[0][0]);
	shader->SetMatrix4(projectionMatrixId, 1, false, &projectionMatrix[0][0]);
	
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	//glDrawArrays(GL_TRIANGLES, 0, m_numOfVertexs);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	glBindVertexArray(0);

	shader->TurnOff();
}