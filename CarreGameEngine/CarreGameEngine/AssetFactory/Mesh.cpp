#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex3> vertices, std::vector<unsigned int> indices)
{
	m_vertices = vertices;
	m_indices = indices;

	SetupMesh();
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex3), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (void*)offsetof(Vertex3, m_normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (void*)offsetof(Vertex3, m_texCoords));

	glBindVertexArray(0);
}

void Mesh::Draw()
{
	//unsigned int diffuseNr = 1;
	//unsigned int specularNr = 1;
	//for (unsigned int i = 0; i < m_textures.size(); i++)
	//{
	//	glActiveTexture(GL_TEXTURE0 + i);	// activate proper texture unit before binding
	//										// retrieve texture number (the N in diffuse_textureN)
	//	std::string number;
	//	std::string name = m_textures[i].type;
	//	if (name == "texture_diffuse")
	//		number = std::to_string(diffuseNr++);
	//	else if (name == "texture_specular")
	//		number = std::to_string(specularNr++);
	//	shader.SetFloat(atoi(("material." + name + number).c_str()), i);
	//	glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
	//}
	//glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//// This turns our shader on before we tell OpenGL to render the triangle.
	//m_shader.TurnOn();

	//// Grab the view and projection matrices from the current camera
	//glm::mat4 projectionMatrix = Camera->GetProjectionMatrix();
	//glm::mat4 viewMatrix = Camera->GetViewMatrix();

	//// Create the Model Matrix with the current position
	//glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_position);

	//// Similar to the Position above, we do the same for the Scale
	//modelMatrix = glm::scale(modelMatrix, m_scale);

	//// Add any rotation to the Model Matrix for each axis
	//modelMatrix = glm::rotate(modelMatrix, m_rotation.x, glm::vec3(1, 0, 0));	// x-axis
	//modelMatrix = glm::rotate(modelMatrix, m_rotation.y, glm::vec3(0, 1, 0));	// y-axis
	//modelMatrix = glm::rotate(modelMatrix, m_rotation.z, glm::vec3(0, 0, 1));	// z-axis

	//// Get the variables from the shader for each matrix
	//GLint modelMatrixId = m_shader.GetVariable("u_modelMatrix");
	//GLint viewMatrixId = m_shader.GetVariable("u_viewMatrix");
	//GLint projectionMatrixId = m_shader.GetVariable("u_projectionMatrix");

	//// Set the model, view and projection matrix for the uniform matrices in the Vertex m_shader.
	//// To get at a GLM glm::mat4 value, we use the address of the first item [0][0].
	//m_shader.SetMatrix4(modelMatrixId, 1, false, &modelMatrix[0][0]);
	//m_shader.SetMatrix4(viewMatrixId, 1, false, &viewMatrix[0][0]);
	//m_shader.SetMatrix4(projectionMatrixId, 1, false, &projectionMatrix[0][0]);

	//// First we want to tell OpenGL with VAO is going to be used for rendering
	//glBindVertexArray(VAO);

	//// Tell OpenGL to enable our vertex information within the m_shaders
	//glEnableVertexAttribArray(0); // kVertexIndex
	//glEnableVertexAttribArray(1); // kNormalIndex
	//glEnableVertexAttribArray(2); // kTextureIndex
	////glEnableVertexAttribArray(3); // kColorIndex

	//// Tell OpenGL to draw the triangles from our VBO
	//glDrawArrays(GL_TRIANGLES, 0, VerticesLength);

	//// Disable indexes in reverse order?
	////glDisableVertexAttribArray(kNormalIndex);
	//glDisableVertexAttribArray(2);
	//glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(0);

	//// Next, let's reset the current VAO binding to 0, which clears out any binding
	//glBindVertexArray(0);

	//// This turns off the m_shader so that other objects rendered after this won't be using this m_shader.
	//m_shader.TurnOff();
}