/**
DogeyBalls
Entity.h
Purpose: Store data and manipulate the entity's
behaivour and state

@author Miguel Saavedra
@version 1.0 6/10/16
*/
#ifndef __ENTITY_H__
#define __ENTITY_H__
#include <string>
#include "ModelData.h"
#include <PxPhysicsAPI.h> 
#include "ActorData.h"

// Forward Declarations:

class Entity
{
	//Member Methods:
public:
	Entity();
	~Entity();

	// Initialie the basic entity object
	void Init(std::string name, const char modelFileLoc,
		float x, float y, float z, int type, float radius);
	// TODO: change place of radius

	// Proccess the state of the Entity at any time
	// during the game
	void Process(float deltaTime);

	void SetDead(bool dead);
	bool IsDead() const;

	// TODO: Implement some type of function to edit coliding state
	bool IsCollidingWith(Entity& e);

	// Get Position
	float GetPositionX()const;
	float GetPositionY()const;
	float GetPositionZ()const;

	// Set Position
	void SetPositionX(float x);
	void SetPositionY(float y);
	void SetPositionZ(float z);

	// Get Velocities
	float GetXVelocity()const;
	float GetYVelocity()const;
	float GetZVelocity()const;

	// Set Velocities
	void SetYVelocity(float y);
	void SetXVelocity(float x);
	void SetZVelocity(float x);

	// TODO: get MODEL pointer

	// TODO: actor functions
	void InitActor(physx::PxRigidDynamic *actor, ActorData* data);

	physx::PxRigidDynamic* GetDynamic();
	physx::PxRigidActor* GetActor();
	void SetActor(physx::PxRigidActor* newActor);
	

protected:

private:
	//Member Data:
public:
	std::string m_name;
	// Type of entity
	// TODO: move to some global variables
	enum type {player, ball, rocket, box};
	int entityType;
	
	// PhysX Actor Variables
	// Cannot create unique because object is protected
	physx::PxRigidActor* actor;
	// Stores initial data of actor
	std::unique_ptr<ActorData> actorData;
	//Stores Rigid Dynamics of Actor
	physx::PxRigidDynamic* dynamic;

protected:
	// Default Physx variables
	float DENSITY = 1.0f; // How dense
	float ADAMPENING = 0.75f; // IDK
	// TODO: move this somewhere where it can be changed on init
	float RADIUS = 5;

	// Position of Entity
	// Extends to Position of model
	float m_x;
	float m_y;
	float m_z;

	// Used to Update the position of the entity
	float m_velocityX;
	float m_velocityY;
	float m_velocityZ;

	// Used to update the game state checked in the update function
	bool m_dead;

	// Model Data Pointer which contains information about the model
	// TODO: add pointer
	ModelData* m_modelData;

private:

};

#endif //__ENTITY_H__
