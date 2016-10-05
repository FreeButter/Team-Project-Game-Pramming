/**
DogeyBalls
PhysXController.h
Purpose: Controls the Physics

@author Miguel Saavedra - Assisted by Sonic
@version 1.0 1/10/16
*/
#ifndef PHYSXCONTROLLER_H
#define PHYSXCONTROLLER_H

#include "PxPhysicsAPI.h"

class PhysXController
{

public: // Public Function Definitions
	PhysXController();
	~PhysXController();

	// Generalarized Template setter
	template <typename T>
	void SetVar(T&& value, T&& originalValue){ value = std::forward<T>(originalValue); }

	void InitPhysX();
	void ReleasePhysX();

<<<<<<< HEAD
	void CreateScene();
=======
	void Simulate();
>>>>>>> origin/PhysX-Implementation

public: // Public Member Definitions

	// Member objects
<<<<<<< HEAD
	physx::PxDefaultErrorCallback* gDefaultErrorCallback;
	physx::PxDefaultAllocator* gDefaultAllocatorCallback;

	//static PxDefaultSimulationFilterShader gDefaultFilterShader;

	physx::PxProfileZoneManager* mProfileZoneManager;
	physx::PxFoundation* mFoundation;
	physx::PxPhysics* mPhysics;
	physx::PxCooking* mCooking;
	physx::PxScene* mScene;
=======
	static physx::PxDefaultErrorCallback gDefaultErrorCallback;
	static physx::PxDefaultAllocator gDefaultAllocatorCallback;

	// Member pointers
	physx::PxPhysics *mPhysics = NULL;
	physx::PxFoundation *mFoundation = NULL;
	physx::PxProfileZoneManager *mProfileZoneManager;

	// Scene Variables

>>>>>>> origin/PhysX-Implementation

	// Member pointers

};

#endif