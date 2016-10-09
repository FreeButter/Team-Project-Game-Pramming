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

class ActorData;

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

	void CreateScene();

	void Simulate();
	void StepPhysX(float time);

	// Rendering Functions
	void DrawShape(physx::PxShape* pShape);
	void Render();
	void DrawActor(physx::PxRigidActor* actor);

	// Init given from the entity class actor
	// and add it to the physx scene
	physx::PxRigidActor* InitActor(physx::PxRigidActor* actor, ActorData* data, physx::PxRigidDynamic* dynamic);

public: // Public Member Definitions

	// Member objects
	std::unique_ptr<DirectX::GeometricPrimitive> m_shape;

	// Member pointers

	// Scene Variables

	// Member pointers

};

#endif