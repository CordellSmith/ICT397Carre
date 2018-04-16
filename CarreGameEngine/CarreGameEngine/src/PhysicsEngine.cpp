/*
* Implementation of PhysicsEngine.h file
* Author - Jack Matters
* Note - Bullet Physics Engine documentation http://bulletphysics.org/wordpress/?page_id=9
*/

//Includes
#include "..\headers\PhysicsEngine.h"
#include <iostream>

// Default constructor
PhysicsEngine::PhysicsEngine()
{
	// Collision configuration contains default setup for memory, collision setup
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	// Use the default collision dispatcher
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// A good general purpose broadphase
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	// The default constraint solver
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	// The dynamic world
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	// Set the gravity
	dynamicsWorld->setGravity(btVector3(0, -2, 0));

	// Initialize all objects to static
	isDynamic = false;

	// Initialize player object location
	playerObject.setZero();

	oldForce.setZero();
	newForce.setZero();
}

// De-constructor
PhysicsEngine::~PhysicsEngine(){};

// Create a static rigid body (ground)
void PhysicsEngine::CreateStaticRigidBody()
{
	// Create a floor shape and add to shape array
	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(100.0), btScalar(0.0), btScalar(100.0)));
	collisionShapes.push_back(groundShape);

	// Initialize transform and location
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0.0, -1.0, 0.0));

	// Set mass (zero for static)
	mass = 0.0;

	// Set dynamic objects to objects with mass that is non-zero
	isDynamic = (mass != 0.0f);

	btVector3 localInertia(0.0, 0.0, 0.0);

	if (isDynamic)
		groundShape->calculateLocalInertia(mass, localInertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	// Set the index for the type of rigid body that is being created
	body->setUserIndex(FLOOR);

	// Add the body to the dynamic world
	dynamicsWorld->addRigidBody(body);
}

void PhysicsEngine::CreatePlayerControlledRigidBody(btVector3 &playerObj)
{
	// Create box shape and add to shape array
	btCollisionShape* camShape = new btBoxShape(btVector3(btScalar(1.0), btScalar(1.0), btScalar(1.0)));
	collisionShapes.push_back(camShape);

	// Create a dynamic object
	btTransform startTransform;
	startTransform.setIdentity();

	// Set mass (non-zero for dynamic)
	mass = 2.0f;

	// Set dynamic objects to objects with mass that is non-zero
	isDynamic = (mass != 0.0f);

	btVector3 localInertia(0.0, 0.0, 0.0);

	if (isDynamic)
		camShape->calculateLocalInertia(mass, localInertia);

	// Set origin of body
	startTransform.setOrigin(playerObj);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, camShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	// Set the index for the type of rigid body that is being created
	body->setUserIndex(CAMERA);

	// Add the body to the dynamic world
	dynamicsWorld->addRigidBody(body);

	// Set new player object coordinates
	playerObject = playerObj;

	// Disable gravity for this object
	body->setGravity(btVector3(0.0, 0.0, 0.0));
}

// Create a dynamic rigid body
void PhysicsEngine::CreateDynamicRigidBody(btVector3 &pos)
{
	// Create box shape and add to shape array
	btCollisionShape* boxShape = new btBoxShape(btVector3(btScalar(1.2), btScalar(1.2), btScalar(1.2)));
	collisionShapes.push_back(boxShape);

	// Create a dynamic object
	btTransform startTransform;
	startTransform.setIdentity();

	// Set mass (non-zero for dynamic)
	mass = 1.0f;

	// Set dynamic objects to objects with mass that is non-zero
	isDynamic = (mass != 0.0f);

	btVector3 localInertia(0.0, 0.0, 0.0);

	if (isDynamic)
		boxShape->calculateLocalInertia(mass, localInertia);

	// Set origin of body
	startTransform.setOrigin(pos);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, boxShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	// Set the index for the type of rigid body that is being created
	body->setUserIndex(BOX);
	
	// Add the body to the dynamic world
	dynamicsWorld->addRigidBody(body);
}

// Simulate the dynamic world
void PhysicsEngine::Simulate(std::vector<btVector3> &bodyPos, btVector3 &playerObj)
{
	dynamicsWorld->stepSimulation(1.f / 60.f, 10);

	// Update positions of all dynamic objects
	for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;

		// Reset forces on player object prior to next step simulation
		if (body->getUserIndex() == CAMERA)
		{
			body->clearForces();
			body->setLinearVelocity(btVector3(0,0,0));

		}

		// Update rigid body positions
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}
		
		// Check to see if player object
		if (body->getUserIndex() == CAMERA)
		{
			// TODO: Make this better (Jack)
			// Apply force in direction camera was moved
			newForce.setX((playerObj.x() - playerObject.x()) * 10000);
			newForce.setY((playerObj.y() - playerObject.y()) * 10000);
			newForce.setZ((playerObj.z() - playerObject.z()) * 10000);
			
			//std::cout << playerObject.x() << " " << playerObject.y() << " " << playerObject.z() << "\n" << std::endl;

			// Update rigid-body location for drawing
			body->applyCentralForce(newForce);
			playerObject = trans.getOrigin();
			playerObj = playerObject;
		}
		else
		{
			// Update object positions for drawing
			bodyPos[j].setX(trans.getOrigin().getX());
			bodyPos[j].setY(trans.getOrigin().getY());
			bodyPos[j].setZ(trans.getOrigin().getZ());
		}	
	}
}

