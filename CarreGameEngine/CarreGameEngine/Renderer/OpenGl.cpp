#include "OpenGl.h"

void OpenGl::Prepare(Model* model, std::string vertShader, std::string fragShader)
{
	int meshBatchSize = model->GetMeshBatch().size();
	for (int i = 0; i < meshBatchSize; i++)
	{
		glGenVertexArrays(1, &model->GetMeshBatch()[i].VAO);
		glGenBuffers(1, &model->GetMeshBatch()[i].VBO);
		glGenBuffers(1, &model->GetMeshBatch()[i].EBO);

		glBindVertexArray(model->GetMeshBatch()[i].VAO);
		glBindBuffer(GL_ARRAY_BUFFER, model->GetMeshBatch()[i].VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(model->GetMeshBatch()[i].GetVertices()[0]) * model->GetMeshBatch()[i].GetVertices().size(), &model->GetMeshBatch()[i].GetVertices()[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->GetMeshBatch()[i].EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->GetMeshBatch()[i].GetIndices().size() * sizeof(unsigned int), &model->GetMeshBatch()[i].GetIndices()[0], GL_STATIC_DRAW);

		// vertex positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(model->GetMeshBatch()[i].GetVertices()[0]), 0);
		// vertex colours
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(model->GetMeshBatch()[i].GetVertices()[0]), (GLvoid*)sizeof(model->GetMeshBatch()[i].GetVertices()[0].m_position));
		// vertex normals
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(model->GetMeshBatch()[i].GetVertices()[0]), (GLvoid*)sizeof(model->GetMeshBatch()[i].GetVertices()[0].m_colour));
		// vertex texture coords
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(model->GetMeshBatch()[i].GetVertices()[0]), (GLvoid*)sizeof(model->GetMeshBatch()[i].GetVertices()[0].m_normal));

		glBindVertexArray(0);
	}

	model->GetShader()->Initialize(vertShader, fragShader);
}

void OpenGl::Render(Model* model)
{
	model->GetShader()->TurnOn();

	int meshBatchSize = model->GetMeshBatch().size();
	for (int i = 0; i < meshBatchSize; i++)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		int textureListSize = model->GetTextures().size();
		for (size_t i = 0; i < textureListSize; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
											  // retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = model->GetTextures()[i].m_type;

			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++);
			else if (name == "texture_height")
				number = std::to_string(heightNr++);

			GLuint textureUniformId = model->GetShader()->GetVariable((name + number).c_str());
			model->GetShader()->SetFloat(textureUniformId, i);
			glBindTexture(GL_TEXTURE_2D, model->GetTextures()[i].m_id);
		}
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 projectionMatrix = model->GetCamera()->GetProjectionMatrix();
		glm::mat4 modelMatrix = CreateTransformationMatrix(model->GetPosition(), model->GetRotation(), model->GetScale());
		glm::mat4 viewMatrix = CreateViewMatrix(model->GetCamera());

		GLint modelMatrixId = model->GetShader()->GetVariable("model");
		GLint viewMatrixId = model->GetShader()->GetVariable("view");
		GLint projectionMatrixId = model->GetShader()->GetVariable("projection");

		model->GetShader()->SetMatrix4(modelMatrixId, 1, false, &modelMatrix[0][0]);
		model->GetShader()->SetMatrix4(viewMatrixId, 1, false, &viewMatrix[0][0]);
		model->GetShader()->SetMatrix4(projectionMatrixId, 1, false, &projectionMatrix[0][0]);

		glBindVertexArray(model->GetMeshBatch()[i].VAO);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glDrawArrays(GL_TRIANGLES, 0, model->GetMeshBatch()[i].GetVertices().size());
		//glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);

		glBindVertexArray(0);

	}
	model->GetShader()->TurnOff();
}
