#include "..\headers\Camera.h"

glm::mat4 Camera::SetPerspective(float fov, float aspectRatio, float near, float far)
{
	m_fov = fov;
	m_aspectRatio = aspectRatio;
	m_nearPlane = near;
	m_farPlane = far;

	m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);

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

	CalculateAngleAroundPlayer();
	float horizontalDistance = CalculateHorizontalDistance();
	float verticalDistance = CalculateVerticalDistance();
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

void Camera::Zoom(float yoffset)
{
	// multiplication factor changes how fast scrolling in and out occurs
	int mf = 2;

	if (glm::degrees(m_fov) >= 1.0 && glm::degrees(m_fov) <= 45.0)
	{
		float change = glm::degrees(m_fov);
		change -= yoffset * mf;
		m_fov = glm::radians(change);
	}
	if (glm::degrees(m_fov) <= 1.0)
		m_fov = glm::radians(1.0);
	if (glm::degrees(m_fov) >= 45.0)
		m_fov = glm::radians(45.0);

	m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
}

void Camera::ChangePitch(float yoffest)
{
	// multiplication factor changes how fast pitch up and down occurs
	float mf = 0.01;
	float pitchChange = yoffest;
	m_pitch -= pitchChange * mf;

	if (m_pitch > glm::radians(70.5f))
		m_pitch = glm::radians(70.5f);
	if (m_pitch < glm::radians(-70.5f))
		m_pitch = glm::radians(-70.5f);
}

void Camera::CalculateCameraPosition(float horizontalDistance, float verticalDistance)
{
	
}

float Camera::CalculateHorizontalDistance()
{
	return 1;
}

float Camera::CalculateVerticalDistance()
{
	return 1;
}

void Camera::CalculateAngleAroundPlayer()
{

}