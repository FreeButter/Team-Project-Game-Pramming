/**
DogeyBalls
ModelController.h
Purpose: Controls the model rendering

@author Miguel Saavedra - Assisted by Sonic
@version 1.0 1/10/16
*/
#ifndef PHYSXCONTROLLER_H
#define PHYSXCONTROLLER_H

#include <PxPhysicsAPI.h>

class PhysXController
{

public: // Public Function Definitions
	PhysXController();
	~PhysXController();

	// Generalarized Template setter
	template <typename T>
	void SetVar(T&& value, T&& originalValue){ value = std::forward<T>(originalValue); }

	// Takes a pointer of the DeviceResources object
	// initialize all the device variables needed 
	// for model loading.
	// TODO: Add functioality to read in data from a json String to create Models
	void InitPhysX();
	void ReleasePhysX();

public: // Public Member Definitions

	// Member objects


	// Member pointers


};

#endif