/**
DogeyBalls
ModelController.cpp
Purpose: Controls the model rendering

@author Miguel Saavedra
@version 1.0 1/10/16
*/

#include "pch.h"
#include "PhysXController.h"

PhysXController::PhysXController()
{

}

PhysXController::~PhysXController()
{

}

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


}