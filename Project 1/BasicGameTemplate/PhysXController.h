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
#include "Entity.h"

class ActorData;

class PhysXController
{

public: // Public Function Definitions
	PhysXController();
	~PhysXController();

	// Generalarized Template setter
	template <typename T>
	void SetVar(T&& value, T&& originalValue){ value = std::forward<T>(originalValue); }

	// Initializes PhysX
	// Where the plane is created
	void InitPhysX();

	// Releases all the resources PhysX uses
	void ReleasePhysX();

	// Physics Simulation method
	// takes actors dynamic and simulate them to move
	void StepPhysX(float time, physx::PxVec3 velocity, physx::PxRigidDynamic* dynamic);

	// Rendering Functions
	void DrawShape(physx::PxShape* pShape);
	void DrawActor(physx::PxRigidDynamic* actor);

	void CreateBoundry(physx::PxVec3 position, physx::PxVec3 scale);

	// Init given from the entity class actor
	// and add it to the physx scene
	physx::PxRigidDynamic* InitActor(std::shared_ptr<Entity> entity);

	// Delete Actor gets given a rigid dynamic
	// then uses it to remove the dynamic from the scene
	void DeleteActor(physx::PxRigidDynamic* actor);

	// Method that will move the player
	// using physics
	void MovePlayer(physx::PxRigidDynamic* dynamic, physx::PxVec3 velocity);

public: // Public Member Definitions

	// Member objects
	std::unique_ptr<DirectX::GeometricPrimitive> m_shape;
	
	// Member pointers
	physx::PxMaterial* m_material;
	
	// Scene Variables
	
	// Member pointers

};

#endif