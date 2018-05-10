#include "..\headers\Camera.h"

glm::mat4 Camera::SetPerspective(float fov, float aspectRatio, float near, float far)
{
	m_projectionMatrix = glm::perspective(fov, aspectRatio, near, far);

	return m_projectionMatrix;
}

void Camera::PositionCamera(float positionX, float positionY, float positionZ, float yaw, float pitch)
{
	m_position = glm::vec3(positionX, positionY, positionZ);
	m_yaw = yaw;
	m_pitch = pitch;
}

glm::mat4 Camera::GetRotationMatrix()
{
	glm::mat4 rotationMatrix(0.5f);

	// Add the Pitch rotation along the x and y axis
	rotationMatrix = glm::rotate(rotationMatrix, m_pitch, glm::vec3(1, 0, 0));
	rotationMatrix = glm::rotate(rotationMatrix, m_yaw, glm::vec3(0, 1, 0));

	return rotationMatrix;
}

glm::mat4 Camera::GetViewMatrix()
{
	// Return a view matrix by multiplying our rotation matrix by the inverse of a translation matrix
	return GetRotationMatrix() * glm::inverse(glm::translate(glm::mat4(), m_position));
}

glm::vec3 Camera::GetView()
{
	// Looking down the negative z-axis.
	glm::vec4 viewVector = glm::inverse(GetRotationMatrix()) * glm::vec4(0, 0, -1, 1);

	return glm::vec3(viewVector);
}

glm::vec3 Camera::GetUp()
{
	glm::vec4 upVector = glm::inverse(GetRotationMatrix()) * glm::vec4(0, 1, 0, 1);

	return glm::vec3(upVector);
}

void Camera::MoveCamera(float speed)
{
	// Get our normalized view vector
	glm::vec3 viewVector = GetView();

	m_position.x += viewVector.x * speed;
	m_position.z += viewVector.z * speed;
}

void Camera::SetViewByMouse(float xOffset, float yOffset)
{
	// Rotate the yaw by the mouse's x offset, multiplied by a sensitivity speed setting
	m_yaw += xOffset * (float)m_mouseSpeed;

	// Rotate the pitch by the mouse's y offset, multiplied by a sensitivity speed setting
	m_pitch += yOffset * (float)m_mouseSpeed;

	if (m_yaw > 2 * PI)
		m_yaw = 0;

	// Yaw capped between 0 and 360
	if (m_yaw < 0)
		m_yaw = (float)(2 * PI);

	// Pitch capped to 75 degrees up and -75 degrees down
	if (m_pitch > glm::radians(70.5f))
		m_pitch = glm::radians(70.5f);
	if (m_pitch < glm::radians(-70.5f))
		m_pitch = glm::radians(-70.5f);
}