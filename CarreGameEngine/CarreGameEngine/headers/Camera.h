		/*
		* @class Camera
		* @brief This is our Camera class to manage user movement in our 3D scene
		*
		* @author
		* @version 1.0
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
		* build from the field of fiew, aspect ratio and near/far planes
		*
		* @param float fov
		* @param float aspectRatio
		* @param float nearPlane 
		* @param float farPlane
		*/
	mat4 SetPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);

		/**
		* @brief This builds and returns a rotation matrix from the yaw and pitch rotations
		* 
		* @return mat4
		*/
	mat4 GetRotationMatrix();

		/**
		* @brief This returns the current ProjectionMatrix
		*
		* @return ProjectionMatrix
		*/
	mat4 GetProjectionMatrix() { return ProjectionMatrix; }

		/**
		* @brief This returns the current view matrix according to the camera's position and rotation
		* @return mat4
		*/
	mat4 GetViewMatrix();

		/**
		* @brief This sets the camera's position
		* @param vec3 position
		*
		* @return void
		*/
	void SetPosition(vec3 position)	{ Position = position; }

		/**
		* @brief This gets the camera's position
		*
		* @return Position
		*/
	vec3 GetPosition()	{ return Position; }

		/**
		* @brief This calculates the current view from the position and rotation matrix
		*
		* @return vec3
		*/
	vec3 GetView();

		/**
		* @brief This returns the camera's up vector (the direction pointing up)
		*
		* @return vec3
		*/
	vec3 GetUp();

		/**
		* @brief This gets the Yaw
		*
		* @return float Yaw
		*/
	float GetYaw() { return Yaw; }
		/**
		* @brief This sets the Yaw 
		* @param float yaw
		*
		* @return void
		*/
	void SetYaw(float yaw)	{ Yaw = yaw; }
		/**
		* @brief This gets the Pitch rotations (in radians)
		*
		* @return Pitch
		*/
	float GetPitch() { return Pitch; }
		/**
		* @brief This sets the Pitch rotations (in radians)
		*
		* @param float pitch
		* @return void
		*/
	void SetPitch(float pitch) { Pitch = pitch; }
	
		/**
		* @brief This sets the camera's movement speed
		* @param double speed
		*
		* @return void
		*/
	void SetSpeed(double speed) { Speed = speed;  }
		/**
		* @brief This gets the camera's movement speed
		*
		* @return Speed
		*/
	double GetSpeed() { return Speed; }

		/**
		* @brief This sets the rotation speed for the camera
		* @param double speed
		*
		* @return void
		*/
	void SetRotationSpeed(double speed) { RotationSpeed = speed; }
		/**
		* @brief This gets the rotation speed for the camera
		*
		* @return RotationSpeed
		*/
	double GetRotationSpeed() { return RotationSpeed; }

		/**
		* @brief This sets the pitch and yaw of our camera from the mouse x and y deltas from the last frame
		* @param float mouseX
		* @param floatmouseY
		*
		* @return void
		*/
	void SetViewByMouse(float mouseX, float mouseY);

		/**
		* @brief This sets the Position and the yaw and pitch rotations (in radians)
		*
		* @param float positionX
		* @param float positionY
		* @param float positionZ
		* @param float yaw
		* @param float pitch
		*
		* @return void
		*/
	void PositionCamera(float positionX, float positionY, float positionZ, float yaw, float pitch);

		/**
		* @brief This will move the camera forward or backwards depending on the speed (negative for backwards)
		*
		* @return speed
		*/
	void MoveCamera(float speed);

protected:

	
	mat4 ProjectionMatrix;
	vec3 Position;		

	float Speed = 5.0;
	float RotationSpeed = 2;						
	double MouseSpeed = 0.001;

	float Yaw = 0;		
	float Pitch = 0;
};


