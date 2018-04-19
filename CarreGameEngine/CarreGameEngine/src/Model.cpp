#include "..\headers\Model.h"

void Model::Initialize(Vertex vertices[], int length, std::string vertShaderString, std::string fragShaderString)
{
	m_shader.Initialize(vertShaderString, fragShaderString);

	m_vertices = vertices;
	m_verticesLength = length;

	GLenum ErrorCheckValue = glGetError();
	
	// Create the Vertex Array Object (VAO)
	glGenVertexArrays(1, &m_vertexArrayObjectId);
	glBindVertexArray(m_vertexArrayObjectId);

	// Create the Vertex Buffer Object (VBO)
	glGenBuffers(1, &m_vertexBufferObjectId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjectId);

	// Store the vertices in the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * length, m_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(m_vertexIndex, 3, GL_FLOAT, GL_FALSE, sizeof(m_vertices[0]), 0);
	glVertexAttribPointer(m_colorIndex, 4, GL_FLOAT, GL_FALSE, sizeof(m_vertices[0]), (GLvoid*)sizeof(m_vertices[0].xyz));
	glVertexAttribPointer(m_textureIndex, 2, GL_FLOAT, GL_FALSE, sizeof(m_vertices[0]), (GLvoid*)sizeof(m_vertices[0].uv));
	glVertexAttribPointer(m_normalIndex, 3, GL_FLOAT, GL_FALSE, sizeof(m_vertices[0]), (GLvoid*)sizeof(m_vertices[0].normal));
	
	ErrorCheckValue = glGetError();

	if ( ErrorCheckValue != GL_NO_ERROR )
	{
		fprintf(stderr, "ERROR: Could not create a VAO and VBO with error Id: %d\n", ErrorCheckValue);
		exit(-1);
	}
}

void Model::Render()
{
	m_shader.TurnOn();

	// Grab the view and projection matrices
	glm::mat4 projectionMatrix = m_camera->GetProjectionMatrix();
	glm::mat4 viewMatrix = m_camera->GetViewMatrix();

	// Create the Model Matrix with the current position
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_positon);

	// Similar to the Position above, we do the same for the m_scale
	modelMatrix = glm::scale(modelMatrix, m_scale);

	// Add any rotation to the Model Matrix for each axis
	modelMatrix = glm::rotate(modelMatrix, m_rotation.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, m_rotation.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, m_rotation.z, glm::vec3(0, 0, 1));

	// Get the uniform variables from the shader
	GLint modelMatrixId = m_shader.GetVariable("u_modelMatrix");
	GLint viewMatrixId = m_shader.GetVariable("u_viewMatrix");
	GLint projectionMatrixId = m_shader.GetVariable("u_projectionMatrix");
	
	// Set the model, view and projection matrix for the uniform matrices in the Vertex Shader.
	// To get at a GLM glm::mat4 value, we use the address of the first item [0][0].
	m_shader.SetMatrix4(modelMatrixId, 1, false, &modelMatrix[0][0]);
	m_shader.SetMatrix4(viewMatrixId, 1, false, &viewMatrix[0][0]);
	m_shader.SetMatrix4(projectionMatrixId, 1, false, &projectionMatrix[0][0]);

	glBindVertexArray(m_vertexArrayObjectId);

	// Enable vertex information
	glEnableVertexAttribArray(m_vertexIndex);
	glEnableVertexAttribArray(m_colorIndex);
	glEnableVertexAttribArray(m_textureIndex);
	glEnableVertexAttribArray(m_normalIndex);

	// Draw the information in VBO
	glDrawArrays(GL_TRIANGLES, 0, m_verticesLength);

	// Disable vertex information
	glDisableVertexAttribArray(m_vertexIndex);
	glDisableVertexAttribArray(m_colorIndex);
	glDisableVertexAttribArray(m_textureIndex);
	glDisableVertexAttribArray(m_normalIndex);

	// Reset the VAO
	glBindVertexArray(0);

	m_shader.TurnOff();
}

void Model::Destroy()
{
	if (m_vertexBufferObjectId)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_vertexBufferObjectId);
		m_vertexBufferObjectId = 0;
	}

	if  (m_vertexArrayObjectId)
	{
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_vertexArrayObjectId);
		m_vertexArrayObjectId = 0;
	}

	m_camera = nullptr;
	m_shader.Destroy();
}