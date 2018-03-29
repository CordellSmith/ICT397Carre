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
* @version 01 Initial start. Took ideas from a previous physics engine I worked on that used ReactPhysics3D. Also used HelloWorld example as a guide.
			  Managed to get 2 spheres (currently shown as floor planes) to collide off each other and a floor.
*/

#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

/// Includes
#include "btBulletDynamicsCommon.h"
#include <vector>

class PhysicsEngine
{
	public:
		/**
		* @brief Default constructor
		*/
		PhysicsEngine();

		/**
		* @brief De-constructor
		*/
		~PhysicsEngine();

		/**
		* @brief Creates static rigid body
		*/
		void CreateStaticRigidBody();

		/**
		* @brief Creates dynamic rigid body
		* @param pos - Position to create dynamic body
		*/
		void CreateDynamicRigidBody(btVector3 &pos);

		/**
		* @brief Simulate the dynamic world
		* @param bodyPos - Update all rigid body positions for drawing
		*/
		void Simulate(std::vector<btVector3> &bodyPos);

	private:

		/// Determines if shape is dynamic or not
		bool isDynamic;

		/// Dynamic world
		btDiscreteDynamicsWorld* dynamicsWorld;

		/// Array of collision shapes
		btAlignedObjectArray<btCollisionShape*> collisionShapes;

		/// Mass value of body
		btScalar mass;

	protected:


};

#endif