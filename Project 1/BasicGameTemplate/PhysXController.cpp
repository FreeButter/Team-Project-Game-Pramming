///**
//DogeyBalls
//ModelController.cpp
//Purpose: Controls the model rendering
//
//@author Miguel Saavedra
//@version 1.0 1/10/16
//*/
//
//#include "pch.h"
//#include "PhysXController.h"
//#include <PxPhysicsAPI.h>
//#include <PxDefaultErrorCallback.h>
//#include <PxDefaultAllocator.h>
//#include <PxExtensionsAPI.h>
//#include <foundation\PxFoundation.h>
//#include <iostream>
//
//PhysXController::PhysXController()
//{
//
//}
//
//PhysXController::~PhysXController()
//{
//
//}
//
//void
//PhysXController::InitPhysX()
//{
//	gDefaultAllocatorCallback = new physx::PxDefaultAllocator();
//	if (!gDefaultAllocatorCallback)
//	{
//		std::cerr << "Unable to create DefaultAllocatorCallback\n";
//		exit(-1);
//	}
//	gDefaultErrorCallback = new physx::PxDefaultErrorCallback();
//	if (!gDefaultErrorCallback)
//	{
//		std::cerr << "Unable to create DefaultErrorCallback\n";
//		exit(-1);
//	}
//
//	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *gDefaultAllocatorCallback, *gDefaultErrorCallback);
//	assert(!mFoundation);
//		//fatalError("PxCreateFoundation failed!");
//
//	bool recordMemoryAllocations = true;
//	//mProfileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(mFoundation);
//	//assert(!mProfileZoneManager);
//	//	//fatalError("PxProfileZoneManager::createProfileZoneManager failed!");
//
//	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
//    physx::PxTolerancesScale(), recordMemoryAllocations, mProfileZoneManager);
//	assert(!mPhysics);
//		//fatalError("PxCreatePhysics failed!");
//
//	//mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, physx::PxCookingParams());
//	//assert(!mCooking);
//	//	//fatalError("PxCreateCooking failed!");
//
//}
//
//void
//PhysXController::ReleasePhysX()
//{
//	mPhysics->release();
//	mFoundation->release();
//}
//
////void
////PhysXController::CreateScene()
////{
////	static PxDefaultSimulationFilterShader gDefaultFilterShader;
////
////	physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
////	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
////	customizeSceneDesc(sceneDesc);
////
////	if (!sceneDesc.cpuDispatcher)
////	{
////		mCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(mNbThreads);
////		if (!mCpuDispatcher)
////			fatalError("PxDefaultCpuDispatcherCreate failed!");
////		sceneDesc.cpuDispatcher = mCpuDispatcher;
////	}
////	if (!sceneDesc.filterShader)
////		sceneDesc.filterShader = &gDefaultFilterShader;
////
////#ifdef PX_WINDOWS
////	if (!sceneDesc.gpuDispatcher && mCudaContextManager)
////	{
////		sceneDesc.gpuDispatcher = mCudaContextManager->getGpuDispatcher();
////	}
////#endif
////
////	mScene = mPhysics->createScene(sceneDesc);
////	if (!mScene)
////		fatalError("createScene failed!");
////
////}
//
//PX_FORCE_INLINE physx::PxSimulationFilterShader getSampleFilterShader()
//{
//	return physx::PxDefaultSimulationFilterShader;
//}

#include "pch.h"
#include "PhysXController.h"

using namespace std;
using namespace physx;

// link libraries
//#pragma comment(lib, "PhysX3CHECKED_x86.lib")
//#pragma comment(lib, "PhysX3CommonCHECKED_x86.lib")
//#pragma comment(lib, "PhysX3ExtensionsCHECKED.lib")
#pragma comment(lib,"PhysX3DEBUG_x86.lib")
#pragma comment(lib,"PhysX3CharacterKinematicDEBUG_x86.lib")
#pragma comment(lib,"PhysX3CommonDEBUG_x86.lib")
#pragma comment(lib,"PhysX3CookingDEBUG_x86.lib")
#pragma comment(lib,"PhysXProfileSDKDEBUG.lib")
#pragma comment(lib,"PxTaskDEBUG.lib")
#pragma comment(lib,"PhysX3ExtensionsDEBUG.lib")

// Physx Physics
PxPhysics *mPhysics = NULL;

void initPhysX() {
	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;
	PxFoundation *mFoundation = NULL;
	PxProfileZoneManager* mProfileZoneManager;

	printf("initializing PhysX\n");

<<<<<<< HEAD
	printf("creating Foundation\n");
	// create foundation object with default error and allocator callbacks.
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);

	//bool recordMemoryAllocations = true;
	//mProfileZoneManager = &PxProfileZoneManager::createProfileZoneManager(mFoundation);
	//assert(!mProfileZoneManager);
	//	//fatalError("PxProfileZoneManager::createProfileZoneManager failed!");

	printf("creating Physics\n");
	// create Physics object with the created foundation and with a 'default' scale tolerance.
	mPhysics = PxCreatePhysics( PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale());
	assert(mPhysics);
	printf("PhysX initialized\n");

	printf("shutting down\n");
	mPhysics->release();
	mFoundation->release();
}
=======
void
PhysXController::InitPhysX()
{
	static physx::PxDefaultErrorCallback gDefaultErrorCallback;
	static physx::PxDefaultAllocator gDefaultAllocatorCallback;

	// Initialize foundation  
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
		gDefaultErrorCallback);
	assert(!mFoundation);
		//fatalError("PxCreateFoundation failed!");

	// top-level PxPhysics object:
	bool recordMemoryAllocations = true;
	mProfileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(mFoundation);
	assert(!mProfileZoneManager);
		//fatalError("PxProfileZoneManager::createProfileZoneManager failed!");

	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
	    physx::PxTolerancesScale(), recordMemoryAllocations, mProfileZoneManager);
	assert(!mPhysics);
		//fatalError("PxCreatePhysics failed!");

	// Initialize Cooking Yummm
	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, physx::PxCookingParams(scale));
	if (!mCooking)
		fatalError("PxCreateCooking failed!");
}

void
PhysXController::ReleasePhysX()
{
	mPhysics->release();
	mFoundation->release();
}


void
PhysXController::Simulate()
{
	scene->simulate(dt);
	scene->fetchResults(true);

>>>>>>> origin/PhysX-Implementation

//int main() {
//	initPhysX();
//
//	return 0;
//}