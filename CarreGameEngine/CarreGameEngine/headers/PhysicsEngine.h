/**
*@note	Bullet Continuous Collision Detection and Physics Library
*		Copyright (c) 2003-2007 Erwin Coumans  http://continuousphysics.com/Bullet/
*
*		This software is provided 'as-is', without any express or implied warranty.
*		In no event will the authors be held liable for any damages arising from the use of this software.
*		Permission is granted to anyone to use this software for any purpose,
*		including commercial applications, and to alter it and redistribute it freely,
*		subject to the following restrictions:
*
*		1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, 
		   an acknowledgment in the product documentation would be appreciated but is not required.
*		2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
*		3. This notice may not be removed or altered from any source distribution.
*
* @class PhysicsEngine
* @brief Physics engine that handles all object collisions and physics based movement
* @author Jack Matters
*
* @date 29/04/2018
* @version 1.0	Initial start. Took ideas from a previous physics engine I worked on that used ReactPhysics3D. Also used HelloWorld example as a guide.
*				Managed to get 2 spheres (currently shown as floor planes) to collide off each other and a floor.
*
* @date 05/04/2018
* @version 2.0	Altered code to draw a square dynamic rigid body instead of a sphere for testing purposes. Started work on camera collision. 
*
* @date 06/04/2018
* @version 2.1  Tidied up camera (player object) code. Force is now applied in direction camera was moved, calculations are done, then camera is moved to updated
*				position. Still needs some work, but will do for now.
* @bug			If camera is not moved for a short period of time, it becomes stuck. Restart application to fix. Will fix this at a later date.
*
* @date 18/04/2018
* @version 3.0	Big gap between versions due to other units requirements. Set up functions for initializing all object rigid bodies. Will need changing.
*
* @date 19/04/2018
* @version 3.1	Added testing for heightfield terrain shape. Got it loading and working, but without a visual terrain it is not possible to see if it is
*				working correctly or not. Have commented it out until I am able to keep working on it. Also added some more doxygen comments.
*/

#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

/// Includes
#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionShapes\btHeightfieldTerrainShape.h"
#include <vector>
#include <fstream>	// Used for testing of heightfield terrain shape (will be removed later)

/**
* @brief Enum for the different types of rigid bodies created.
*
* Enum used to differentiate between the different types of rigid bodies created
*
* @param CAMERA - Camera object rigid body
* @param BOX - Box shape rigid body
* @param SPHERE - Sphere shape rigid body
* @param HEIGHTFIELD - Heightfield terrain shape rigid body
* @param PLANE - Plane shape rigid body
*/
typedef enum
{
	CAMERA = 1,
	BOX = 2,
	SPHERE = 3,
	HEIGHTFIELD = 4,
	PLANE = 5
}RIGID_BODY_TYPE;

class PhysicsEngine
{
	public:

		/**
		* @brief Default constructor
		* 
		* This is the default constructor
		*
		* @return null
		*/
		PhysicsEngine();

		/**
		* @brief De-constructor
		*
		* This is the de-constructor
		*
		* @note Not implemented yet
		*
		* @return null
		*/
		~PhysicsEngine();

		/**
		* @brief Creates static rigid body
		*
		* This is a test function that is used to create a static body (as of now, it creates a floor)
		*
		* @return void
		*/
		void CreateStaticRigidBody();

		/**
		* @brief Creates dynamic rigid body
		*
		* This is a test function that creates dynamic rigid bodies for testing purposes
		*
		* @param pos - Position to create dynamic body
		*
		* @return void
		*/
		void CreateDynamicRigidBody(btVector3 &pos);

		/**
		* @brief Creates dynamic rigid body for a player controlled object
		*
		* This is a test function that creates a dynamic rigid body for the player controlled object (as of now, that is the camera)
		* 
		* @param playerObj - Object that is player controlled 
		*
		* @return void
		*/
		void CreatePlayerControlledRigidBody(btVector3 &playerObj);

		/**
		* @brief Simulate the dynamic world
		*
		* This function simulates the dynamic world by handling all physics calculations each step
		*
		* @param bodyPos - Update all rigid body positions for drawing
		* @param playerObj - Sets new player object position
		*
		* @return void
		*/
		void Simulate(std::vector<btVector3> &bodyPos, btVector3 &playerObj);

		/*
		* @brief Public function that calls different private functions for creation of rigid bodies
		* @param objectData - Data structure containing all game object data
		* @return True if all game object rigid bodies created, false otherwise
		*/
		//bool CreateAllRigidBodies(Data &objectData);

		/**
		* @brief Create a heightfield terrain shape
		*
		* This is a test function that creates a heightfield terrain shape
		*
		* @return void
		*/
		void CreateHeightfieldTerrainShape();

	private:

		/// Determines if shape is dynamic or not
		bool m_isDynamic;

		/// Dynamic world
		btDiscreteDynamicsWorld* m_dynamicsWorld;

		/// Array of collision shapes
		btAlignedObjectArray<btCollisionShape*> m_collisionShapes;

		/// Mass value of body
		btScalar m_mass;

		/// Holds last known player controlled object location
		btVector3 m_playerObject;

		/// Old force applied to player controlled object (don't think this is needed)
		btVector3 m_oldForce;

		/// New force applied to player controlled object (don't think this is needed)
		btVector3 m_newForce;

		/*
		* @brief Creates a rigid body for the camera
		* @param objectData - Data structure containing all data for rigid body creation
		*/
		//void CreateCameraRigidBody(Data &objectData);

		/*
		* @brief Creates a box shape rigid body
		* @param objectData - Data structure containing all data for rigid body creation
		*/
		//void CreateBoxShapeRigidBody(Data &objectData);

		/*
		* @brief Creates a sphere shape rigid body
		* @param objectData - Data structure containing all data for rigid body creation
		*/
		//void CreateSphereShapeRigidBody(Data &objectData);

		/*
		* @brief Creates a heightfield terrain shape
		* @param objectData - Data structure containing all data for rigid body creation
		*/
		//void CreateHeightFieldTerrainShape(Data &objectData);

		/// Size of heightfield data (used for testing)
		int m_size;

		/// Holds all heightfield data (used for testing)
		unsigned char *m_terrainData;

	protected:


};

#endif