#pragma once

#define PI 3.14159265358979323846264338327950288		// Define PI for usage in positioning the camera's rotations
#define GLM_FORCE_RADIANS								// Make sure GLM is using radians instead of degrees

#include <fstream>
#include <GLM\glm.hpp>									// Used for the GLM math library
#include <GLM\gtc\matrix_transform.hpp>					// Used for the GLM math library
#include <GLM\gtx\transform2.hpp>						// Used for the GLM math library
using namespace glm;

// This is our Camera class to manage user movement in our 3D scene
class Camera 
{
public:

	Camera();

	// This sets and returns a perspective matrix that is build from the field of fiew, aspect ratio and near/far planes
	mat4 SetPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);

	// This builds and returns a rotation matrix from the yaw and pitch rotations
	mat4 GetRotationMatrix();

	// This returns the current ProjectionMatrix
	mat4 GetProjectionMatrix() { return ProjectionMatrix; }

	// This returns the current view matrix according to the camera's position and rotation
	mat4 GetViewMatrix();

	// This sets and gets the camera's position
	void SetPosition(vec3 position)	{ Position = position; }
	vec3 GetPosition()	{ return Position; }

	// This calculates the current view from the position and rotation matrix
	vec3 GetView();

	// This returns the camera's up vector (the direction pointing up)
	vec3 GetUp();

	// This sets and gets the Yaw and Pitch rotations (in radians)
	float GetYaw() { return Yaw; }
	void SetYaw(float yaw)	{ Yaw = yaw; }
	float GetPitch() { return Pitch; }
	void SetPitch(float pitch) { Pitch = pitch; }
	
	// This sets and gets the camera's movement speed
	void SetSpeed(double speed) { Speed = speed;  }
	double GetSpeed() { return Speed; }

	// This gets and sets the rotation speed for the camera
	void SetRotationSpeed(double speed) { RotationSpeed = speed; }
	double GetRotationSpeed() { return RotationSpeed; }

	// This sets the pitch and yaw of our camera from the mouse x and y deltas from the last frame
	void SetViewByMouse(float mouseX, float mouseY);

	// This sets the Position and the yaw and pitch rotations (in radians)
	void PositionCamera(float positionX, float positionY, float positionZ, float yaw, float pitch);

	// This will move the camera forward or backwards depending on the speed (negative for backwards)
	void MoveCamera(float speed);

protected:

	mat4 ProjectionMatrix;								// The camera's projection matrix
	vec3 Position;										// The camera's position

	float Speed = 5.0;									// The camera's speed that is used with a time slice
	float RotationSpeed = 2;							// The rotation speed for the camera

	double MouseSpeed = 0.001;							// The speed for the mouse rotations

	float Yaw = 0;										// The horizontal rotation angle (in radians), y-axis
	float Pitch = 0;									// The vertical rotation angle (in radians, x-axis
};


