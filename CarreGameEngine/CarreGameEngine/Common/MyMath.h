#pragma once

#include <iostream>
#include <GLM\glm.hpp>									// Used for the GLM math library
#include <GLM\gtc\matrix_transform.hpp>					
#include <GLM\gtx\transform2.hpp>		

class MyMath
{
public:
	MyMath() { }
	~MyMath() { }

	//glm::mat4 CreateTransformationMatrix(glm::vec3 postion, glm::vec3 rotation, glm::vec3 scale);

protected:

};

inline glm::mat4 CreateTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
{
	glm::mat4 modelMatrix;
	// Order is important, must be Translate, Scale then Rotation
	// TRANSLATE
	modelMatrix = glm::translate(glm::mat4(0.5f), translation);

	// SCALE
	modelMatrix = glm::scale(modelMatrix, scale);

	// ROTATE
	//modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
	//modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
	//modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));

	return modelMatrix;
}