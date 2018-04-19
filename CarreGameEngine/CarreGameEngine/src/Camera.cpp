#include "..\headers\Camera.h"

/*
In this tutorial we added a way for us to rotate our camera by moving the mouse around, like
what you would see in a first person game.  This is done by just exposing a new 
SetViewByMouse() function which takes a mouse x and y delta to rotate the yaw and pitch from.
We also can cap the rotations to make sure we don't allow the user to look to high or below.
*/

// Initialize the camera to have a position at the origin
Camera::Camera()
{
	// Set the default position for the camera to the origin
	Position = glm::vec3(0.0, 0.0, 0.0);
}


// This sets and returns the camera's perspective matrix from a field of view (in radians), aspect ratio 
// and the near and far planes which cull the geometry outside our desired viewing distance.
glm::mat4 Camera::SetPerspective(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	// Create the projection matrix with GLM's perspective function, replacing gluPerspective.
	ProjectionMatrix = perspective(fov, aspectRatio, nearPlane, farPlane);

	return ProjectionMatrix;
}


// This function sets the camera's position and rotation values
void Camera::PositionCamera(float positionX, float positionY, float positionZ, float yaw, float pitch)
{
	// Assign our camera variables
	Position = glm::vec3( positionX, positionY, positionZ );
	Yaw = yaw;
	Pitch = pitch;
}


// This builds and returns a rotation matrix from the yaw and pitch rotations
glm::mat4 Camera::GetRotationMatrix()
{
	// Create an identity matrix
	glm::mat4 rotationMatrix(1.0f);

	// Add the Pitch rotation along the x-axis
	rotationMatrix = rotate(rotationMatrix, Pitch, glm::vec3(1, 0, 0));

	// Add the Yaw rotation along the y-axis
	rotationMatrix = rotate(rotationMatrix, Yaw, glm::vec3(0, 1, 0));

	// Return the final rotation matrix that stores our camera rotations
	return rotationMatrix;
}


// This returns the camera's view matrix that is used to pass to our shaders
glm::mat4 Camera::GetViewMatrix()
{
	// Return a view matrix by multiplying our rotation matrix by the inverse of a translation matrix
	return GetRotationMatrix() * inverse(translate(glm::mat4(), Position));
}


// This returns the current view vector (the direction the camera is looking) from the rotation matrix
glm::vec3 Camera::GetView()
{
	// Calculate the view vector by taking the inverse of our rotation matrix multiplied by a vector 
	// looking down the negative z-axis.
	glm::vec4 viewVector = inverse(GetRotationMatrix()) * glm::vec4(0, 0, -1, 1);

	// We are using a 4x4 matrix so our result is a glm::vec4, but just turn it back into a glm::vec3
	return glm::vec3(viewVector);
}


// This returns the camera's up vector (the direction pointing up) from the rotation matrix
glm::vec3 Camera::GetUp()
{
	// Calculate the up vector by taking the inverse of the rotation matrix multiplied by a 
	// default up vector.
	glm::vec4 upVector = inverse(GetRotationMatrix()) * glm::vec4(0, 1, 0, 1);

	// We are using a 4x4 matrix so our result is a glm::vec4, but just cast it back to a glm::vec3
	return glm::vec3(upVector);
}


// This will move the camera forward or backward depending on the speed
void Camera::MoveCamera(float speed)
{
	// Get our normalized view vector (The direction we are facing)
	glm::vec3 viewVector = GetView();

	// Move our camera position forward or backward along the view vector
	Position.x += viewVector.x * speed;		// Add our acceleration to our position's X
	Position.z += viewVector.z * speed;		// Add our acceleration to our position's Z
}

// This sets the pitch and yaw of our camera from the mouse x and y deltas from the last frame
void Camera::SetViewByMouse(float xOffset, float yOffset)
{
	// Rotate the yaw by the mouse's x offset, multiplied by a sensitivity speed setting
	Yaw += xOffset * MouseSpeed;

	// Rotate the pitch by the mouse's y offset, multiplied by a sensitivity speed setting
	Pitch += yOffset * MouseSpeed;

	// If the yaw goes above 360 degrees (2 PI in radians) then set it back to 0 so
	// we keep the rotation between 0 and 360 degrees.  This is just a preference of working
	// with 0-360 instead of 570+, etc...
	if ( Yaw > 2 * PI )
		Yaw = 0;

	// Cap the yaw between 0 and 360
	if ( Yaw < 0 )
		Yaw = 2 * PI;

	// Let's cap the pitch to 75 degrees up and -75 degrees down.  This makes it so you don't
	// have the camera flipping upside down if you look to far up or down.  There could be a
	// reason though to want to allow that, especially in a scripting camera.  This also can
	// avoid the "gimbal lock" issue where if you look straight up or down it wigs out.  This
	// can be avoided with quaternion rotations instead of euler rotations too.
	if ( Pitch > radians(71.0f) )
		Pitch = radians(71.0f);
	if ( Pitch < radians(-71.0f) )
		Pitch = radians(-71.0f);
}