/*
* @class Camera
* @brief This is our Camera class to manage user movement in our 3D scene
*
* @date 29/04/2018
*/
#pragma once

#define PI 3.14159265358979323846264338327950288		// Define PI for usage in positioning the camera's rotations
#define GLM_FORCE_RADIANS								// Make sure GLM is using radians instead of degrees

#include <fstream>
#include <GLM\glm.hpp>									// Used for the GLM math library
#include <GLM\gtc\matrix_transform.hpp>					// Used for the GLM math library
#include <GLM\gtx\transform2.hpp>						// Used for the GLM math library
using namespace glm;


class Camera 
{
public:

	Camera();
	/**
	* @brief This sets and returns a perspective matrix that is 
		build from the field of fiew, aspect ratio and near/far planes
	* @param fov - field of view
	* @param aspectRatio - Aspect ratio
	* @param nearPlane - 
	* @param farPlane -
	*/
	mat4 SetPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);

	/**
	* @brief This builds and returns a rotation matrix from the yaw and pitch rotations
	*/
	mat4 GetRotationMatrix();

	/**
	* @brief This returns the current ProjectionMatrix
	* @return ProjectionMatrix
	*/
	mat4 GetProjectionMatrix() { return ProjectionMatrix; }

	/**
	* @brief This returns the current view matrix according to the camera's position and rotation
	*/
	mat4 GetViewMatrix();

	/**
	* @brief This sets the camera's position
	* @param position - Camera position vector
	*/
	void SetPosition(vec3 position)	{ Position = position; }

	/**
	* @brief This gets the camera's position
	* @return Position
	*/
	vec3 GetPosition()	{ return Position; }

	/**
	* @brief This calculates the current view from the position and rotation matrix
	*/
	vec3 GetView();

	/**
	* @brief This returns the camera's up vector (the direction pointing up)
	*/
	vec3 GetUp();

	/**
	* @brief This gets the Yaw
	* @return Yaw
	*/
	float GetYaw() { return Yaw; }
	/**
	* @brief This sets the Yaw 
	* @param yaw
	*/
	void SetYaw(float yaw)	{ Yaw = yaw; }
	/**
	* @brief This gets the Pitch rotations (in radians)
	* @return Pitch
	*/
	float GetPitch() { return Pitch; }
	/**
	* @brief This sets the Pitch rotations (in radians)
	* @param pitch
	*/
	void SetPitch(float pitch) { Pitch = pitch; }
	
	/**
	* @brief This sets the camera's movement speed
	* @param speed
	*/
	void SetSpeed(double speed) { Speed = speed;  }
	/**
	* @brief This gets the camera's movement speed
	* @return Speed
	*/
	double GetSpeed() { return Speed; }

	/**
	* @brief This sets the rotation speed for the camera
	* @param speed
	*/
	void SetRotationSpeed(double speed) { RotationSpeed = speed; }
	/**
	* @brief This gets the rotation speed for the camera
	* @return RotationSpeed
	*/
	double GetRotationSpeed() { return RotationSpeed; }

	/**
	* @brief This sets the pitch and yaw of our camera from the mouse x and y deltas from the last frame
	* @param mouseX
	* @param mouseY
	*/
	void SetViewByMouse(float mouseX, float mouseY);

	/**
	* @brief This sets the Position and the yaw and pitch rotations (in radians)
	* @param positionX
	* @param positionY
	* @param positionZ
	* @param yaw
	* @param pitch
	*/
	void PositionCamera(float positionX, float positionY, float positionZ, float yaw, float pitch);

	/**
	* @brief This will move the camera forward or backwards depending on the speed (negative for backwards)
	* @return speed
	*/
	void MoveCamera(float speed);

protected:

	/**
	* @brief The camera's projection matrix
	*/
	mat4 ProjectionMatrix;
	/**
	* @brief The camera's position
	*/
	vec3 Position;		

	/**
	* @brief The camera's speed that is used with a time slice
	*/
	float Speed = 5.0;
	/**
	* @brief The rotation speed for the camera
	*/
	float RotationSpeed = 2;						

	/**
	* @brief The speed for the mouse rotations
	*/
	double MouseSpeed = 0.001;
	/**
	* @brief The horizontal rotation angle (in radians), y-axis
	*/
	float Yaw = 0;		
	/**
	* @brief The vertical rotation angle (in radians, x-axis
	*/
	float Pitch = 0;
};


