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

	void CreateScene();

	void Simulate();
	void StepPhysX(float time);

public: // Public Member Definitions

	// Member objects


	// Member pointers

	// Scene Variables

	// Member pointers

};

#endif