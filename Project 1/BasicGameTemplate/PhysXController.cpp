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
#include "vld.h"

using namespace std;
using namespace physx;

// link libraries
#pragma comment(lib,"PhysX3_x86.lib")
#pragma comment(lib,"PhysX3CharacterKinematic_x86.lib")
#pragma comment(lib,"PhysX3Common_x86.lib")
#pragma comment(lib,"PhysX3Cooking_x86.lib")
#pragma comment(lib,"PhysXProfileSDK.lib")
#pragma comment(lib,"PxTask.lib")
#pragma comment(lib,"PhysX3Extensions.lib")


// PhysX Variables note: Does not work if in h file
static physx::PxPhysics* gPhysicsSDK;
static physx::PxFoundation * gFoundationSDK;
static physx::PxProfileZoneManager *gProfileSDK;
static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxDefaultAllocator gDefaultAllocatorCallback;
static physx::PxSimulationFilterShader gDefaultFilterShader = physx::PxDefaultSimulationFilterShader;
physx::PxScene* gScene = NULL;
physx::PxReal myTimestep = 1.0f / 60.0f;
bool bandaid = false;
//Debug
PxContactPairHeader* pairh;
PxContactPair* contactPair;

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

	// Create the scene
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

	// Ground Plane Creation
	PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));

	//Set the Material
	PxMaterial* actMaterial = gPhysicsSDK->createMaterial(0.5, 0.5, 0.5);

	PxRigidStatic* plane = gPhysicsSDK->createRigidStatic(pose);
	PxShape* shape = plane->createShape(PxPlaneGeometry(), *actMaterial);

	gScene->addActor(*plane);
	// Hard Coded Boundries
	// Left Side Boundry
	CreateBoundry(PxVec3(10.0f, 0, -2.0f), PxVec3(0.5f, 10.f, 22.0f));
	// Right Side Boundry
	CreateBoundry(PxVec3(-10.0f, 0, -2.0f), PxVec3(0.5f, 10.f, 22.0f));
	// Top Side Boundry
	CreateBoundry(PxVec3(0.0f, 0, 16.5f), PxVec3(12.f, 8.f, 10.5f));
	// Botom Side Boundry
	CreateBoundry(PxVec3(0.0f, 0, -18.5f), PxVec3(13.f, 8.f, 10.5f));

	// init Dynamics material
	m_material = gPhysicsSDK->createMaterial(0.5, 0.5, 0.5);

	gScene->getSimulationEventCallback();
}

// takes actor and dynamic from entity
// and initializes them
// TODO: Delete actor or use it.
physx::PxRigidDynamic*
PhysXController::InitActor(std::shared_ptr<Entity> entity)
{

	physx::PxRigidDynamic* dynamic = entity->GetDynamic();
	ActorData* data = entity->actorData.get();
	const char* name = entity->m_name.c_str();

	// Material
	if (data->m_type == Entity::ball)
	{
		PxSphereGeometry geometry(data->m_sphereGeometry);
		dynamic = PxCreateDynamic(*gPhysicsSDK, data->m_transform, data->m_sphereGeometry, *m_material, data->m_density);

	}
	if (data->m_type == Entity::box)
	{
		PxBoxGeometry geometry(data->m_boxGeometry);
		dynamic = PxCreateDynamic(*gPhysicsSDK, data->m_transform, data->m_boxGeometry, *m_material, data->m_density);
	}



	// Take all variables from Actor Data object and init actor
	dynamic->setLinearDamping(1.5);
	dynamic->setAngularDamping(data->m_angularDampening);
	dynamic->setLinearVelocity(data->m_linearVelocityVector);
	dynamic->setMass(1);
		
	if (!dynamic)
		cerr << "create actor failed!" << endl;

	// Store actor in scene
	gScene->addActor(*dynamic);

	// Return Dynamic
	return dynamic;
}

void
PhysXController::DeleteActor(physx::PxRigidDynamic* actor)
{
	gScene->removeActor(*actor);
}

// Basic method to draw one shape
void PhysXController::DrawShape(PxShape* pShape) {

	PxTransform pT = PxShapeExt::getGlobalPose(*pShape);
	PxBoxGeometry bg;
	pShape->getBoxGeometry(bg);
	PxMat33 m = PxMat33(pT.q);
}

// Loops through all the shapes and draws them
void PhysXController::DrawActor(PxRigidDynamic* actor)
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
PhysXController::StepPhysX(float time, physx::PxVec3 velocity, PxRigidDynamic* dynamic)
{
	gScene->simulate(myTimestep);
	// Add all physics movement here
	/*onContact(*pairh, contactPair, 1);*/
	physx::PxVec3 mVector = physx::PxVec3(velocity);
	dynamic->addForce(mVector, physx::PxForceMode::Enum::eVELOCITY_CHANGE, true);
	//pDynamic->addTorque(mVector, physx::PxForceMode::Enum::eVELOCITY_CHANGE, true);
	//...perform useful work here using previous frame's state data        
	while (!gScene->fetchResults())
	{
		//TODO: do something useful        
	}
}

void
PhysXController::MovePlayer(physx::PxRigidDynamic* dynamic, physx::PxVec3 velocity)
{
	dynamic->addForce(velocity, physx::PxForceMode::Enum::eFORCE, true);
}

void 
PhysXController::CreateBoundry(PxVec3 position, PxVec3 scale)
{
	PxMaterial* bMaterial = gPhysicsSDK->createMaterial(0.5, 0.5, 0.5);
	PxRigidStatic* boundry;
	PxBoxGeometry boxboundry = PxVec3(scale);

	PxTransform pose = PxTransform(PxVec3(position), PxQuat(PxHalfPi, PxVec3(1.0f, 0.0f, 0.0f)));

	//Set the Material
	PxMaterial* actMaterial = gPhysicsSDK->createMaterial(0.5, 0.5, 0.5);
	
	boundry = gPhysicsSDK->createRigidStatic(pose);
	PxShape* shape = boundry->createShape(boxboundry, *bMaterial);
	//boundry->

	gScene->addActor(*boundry);
}

void
PhysXController::ReleasePhysX()
{
	//Release PhysX Resources
	gPhysicsSDK->release();
	gFoundationSDK->release();
}
