/**
DogeyBalls
PhysXController.h
Purpose: Controls the model rendering

@author Sonic - Assisted By Miguel
@version 1.0 1/10/16
*/

//#includes
#include "pch.h"
#include "PhysXController.h"
#include <iostream>
#include <PxPhysicsAPI.h> 
#include <PxExtensionsAPI.h> 
#include <PxDefaultErrorCallback.h>
#include <PxDefaultAllocator.h> 
#include <PxDefaultSimulationFilterShader.h>
#include <PxDefaultCpuDispatcher.h>
#include <PxSimpleFactory.h>
#include <foundation/PxFoundation.h>
#include <PxRigidStatic.h>
#include "ActorData.h"
#include "Entity.h"

using namespace std;
using namespace physx;

// link libraries
#pragma comment(lib,"PhysX3DEBUG_x86.lib")
#pragma comment(lib,"PhysX3CharacterKinematicDEBUG_x86.lib")
#pragma comment(lib,"PhysX3CommonDEBUG_x86.lib")
#pragma comment(lib,"PhysX3CookingDEBUG_x86.lib")
#pragma comment(lib,"PhysXProfileSDKDEBUG.lib")
#pragma comment(lib,"PxTaskDEBUG.lib")
#pragma comment(lib,"PhysX3ExtensionsDEBUG.lib")


// PhysX Variables note: Does not work if in 
static physx::PxPhysics* gPhysicsSDK;
static physx::PxFoundation * gFoundationSDK;
static physx::PxProfileZoneManager *gProfileSDK;
static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxDefaultAllocator gDefaultAllocatorCallback;
static physx::PxSimulationFilterShader gDefaultFilterShader = physx::PxDefaultSimulationFilterShader;
physx::PxScene* gScene = NULL;
physx::PxReal myTimestep = 1.0f / 60.0f;
physx::PxRigidActor *box;
//PxShape* aSphereShape;
PxMaterial* mMaterial;

PhysXController::PhysXController()
{

}

PhysXController::~PhysXController()
{

}

void
PhysXController::InitPhysX()
{
	//Initialize Foundation
	gFoundationSDK = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if (gFoundationSDK == NULL) {
		cerr << "Error creating PhysX3 device." << endl;
		cerr << "Exiting..." << endl;
		exit(1);
	}

	//Initialize Profile
	bool recordMemoryAllocations = true;
	gProfileSDK = &PxProfileZoneManager::createProfileZoneManager(gFoundationSDK);
	if (gProfileSDK == NULL) {
		cerr << "Error creating PhysX3 device." << endl;
		cerr << "Exiting..." << endl;
		exit(1);
	}

	//Initialize Physics
	gPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundationSDK,
		PxTolerancesScale(), recordMemoryAllocations, gProfileSDK);
	if (gPhysicsSDK == NULL) {
		cerr << "Error creating PhysX3 device." << endl;
		cerr << "Exiting..." << endl;
		exit(1);
	}

	//If PxInitExtensions does not load
	if (!PxInitExtensions(*gPhysicsSDK))
		cerr << "PxInitExtensions failed!" << endl;

	//Visual Debugger Connector
	//PxExtensionVisualDebugger::connect(gPhysicsSDK->getPvdConnectionManager(),"localhost",5425, 10000, true);

	//Create the scene
	PxSceneDesc sceneDesc(gPhysicsSDK->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);

	if (!sceneDesc.cpuDispatcher) {
		PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		if (!mCpuDispatcher)
			cerr << "PxDefaultCpuDispatcherCreate failed!" << endl;
		sceneDesc.cpuDispatcher = mCpuDispatcher;
	}
	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = gDefaultFilterShader;

	gScene = gPhysicsSDK->createScene(sceneDesc);
	if (!gScene)
		cerr << "createScene failed!" << endl;

	gScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0);
	gScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);

	//Set the MAterial
	mMaterial = gPhysicsSDK->createMaterial(0.5, 0.5, 0.5);

	//Create actors 
	//1) Create ground plane
	PxReal d = 0.0f;
	PxTransform pose = PxTransform(PxVec3(0.0f, 1, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));

	//PxRigidStatic* plane = gPhysicsSDK->createRigidStatic(pose);
	//assert(!plane);
		//cerr << "create plane failed!" << endl;

	PxRigidStatic* plane = PxCreatePlane(*gPhysicsSDK, PxPlane(PxVec3(0, 1, 0), 0), *mMaterial);

	PxShape* shape = plane->createShape(PxPlaneGeometry(), *mMaterial);
	//assert(!shape);
		//cerr << "create shape failed!" << endl;
	gScene->addActor(*plane);

	////2) Create cube	 
	//PxReal density = 1.0f;
	//PxTransform transform(PxVec3(0.0f, 10.0f, 0.0f), PxQuat::createIdentity());
	//PxVec3 dimensions(0.5, 0.5, 0.5);
	////PxBoxGeometry geometry(dimensions);
	//// Creating Sphere Geometry
	//PxSphereGeometry geometry(4);

	//PxRigidDynamic *actor = PxCreateDynamic(*gPhysicsSDK, transform, geometry, *mMaterial, density);
	//actor->setAngularDamping(0.75);
	//actor->setLinearVelocity(PxVec3(0, 0, 0));
	//if (!actor)
	//	cerr << "create actor failed!" << endl;
	//gScene->addActor(*actor);

	//aSphereShape = actor;

}

physx::PxRigidActor*
PhysXController::InitActor(physx::PxRigidActor* actor, ActorData* data, PxRigidDynamic* dynamic)
{
	PxRigidDynamic* pDynamic = 0;
	// Creating Sphere Geometry
	if (data->m_type == Entity::ball)
	{
		PxSphereGeometry geometry(data->m_sphereGeometry);
		pDynamic = PxCreateDynamic(*gPhysicsSDK, data->m_transform, data->m_sphereGeometry, *mMaterial, data->m_density);
	}
	if (data->m_type == Entity::box)
	{
		PxBoxGeometry geometry(data->m_boxGeometry);
		pDynamic = PxCreateDynamic(*gPhysicsSDK, data->m_transform, data->m_boxGeometry, *mMaterial, data->m_density);
	}
	
	// Take all variables from Actor Data object and init actor
	pDynamic->setAngularDamping(data->m_angularDampening);
	pDynamic->setLinearVelocity(data->m_linearVelocityVector);
	if (!pDynamic)
		cerr << "create actor failed!" << endl;
	// Store actor in scene
	gScene->addActor(*pDynamic);
	// Set actor to dynamic
	actor = pDynamic;
	
	return actor;
}

void PhysXController::DrawShape(PxShape* pShape) {

	PxTransform pT = PxShapeExt::getGlobalPose(*pShape);
	PxBoxGeometry bg;
	pShape->getBoxGeometry(bg);
	PxMat33 m = PxMat33(pT.q);
}

void PhysXController::DrawActor(PxRigidActor* actor)
{
	// TODO: loop through actor vector
	PxU32 nShapes = actor->getNbShapes();
	PxShape** shapes = new PxShape*[nShapes];

	actor->getShapes(shapes, nShapes);
	while (nShapes--)
	{
		DrawShape(shapes[nShapes]);
	}
	delete[] shapes;
}

void
PhysXController::StepPhysX(float time)
{
	gScene->simulate(time);

	//...perform useful work here using previous frame's state data        
	while (!gScene->fetchResults())
	{
		//TODO: do something useful        
	}
}

void
PhysXController::Render(physx::PxRigidActor* actor)
{
	//PxVec3 position = { 0.0f, 1.0f, 0.0f };
	//PxVec3 velocity = { 0.0f, -0.1f, 0.0f };
	//PxRigidDynamic* aSphereActor = gPhysicsSDK->createRigidDynamic(PxTransform(position));
	//aSphereShape = aSphereActor->createShape(PxSphereGeometry(1), *mMaterial);
	//PxRigidBodyExt::updateMassAndInertia(*aSphereActor, 3);

	//aSphereActor->setLinearVelocity(velocity);

	//gScene->addActor(*aSphereActor);

	//PxRigidStatic* plane = PxCreatePlane(*gPhysicsSDK, PxPlane(PxVec3(0, 1, 0), 0), *mMaterial);

	//gScene->addActor(*plane);
	
	//DrawActor(aSphereShape);

	DrawActor(actor);
}


void
PhysXController::ReleasePhysX()
{
	//Release PhysX Resources
	gPhysicsSDK->release();
	gFoundationSDK->release();
}