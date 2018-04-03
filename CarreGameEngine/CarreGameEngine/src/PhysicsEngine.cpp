/*
* Implementation of PhysicsEngine.h file
* Author - Jack Matters
* Note - Bullet Physics Engine documentation http://bulletphysics.org/wordpress/?page_id=9
*/

//Includes
#include "..\headers\PhysicsEngine.h"

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

	// Add the body to the dynamic world
	dynamicsWorld->addRigidBody(body);
};

// Create a dynamic rigid body
void PhysicsEngine::CreateDynamicRigidBody(btVector3 &pos)
{
	// Create sphere shape and add to shape array
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

	// Add the body to the dynamic world
	dynamicsWorld->addRigidBody(body);
}

// Simulate the dynamic world
void PhysicsEngine::Simulate(std::vector<btVector3> &bodyPos)
{
	dynamicsWorld->stepSimulation(1.f / 60.f, 10);

	// Update positions of all dynamic objects
	for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}
		
		// Update object positions for drawing
		bodyPos[j].setX(trans.getOrigin().getX());
		bodyPos[j].setY(trans.getOrigin().getY());
		bodyPos[j].setZ(trans.getOrigin().getZ());
	}
}

