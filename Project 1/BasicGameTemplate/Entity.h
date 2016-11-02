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
#include "Parser.h"

// Forward Declarations:
class CapturedInputData;

class Entity
{
	//Member Methods:
public:
	Entity();
	~Entity();

	// Initialie the basic entity object
	void Init(const Value& json);

	//Initializes the dodgeball
	void InitBall(const Value& json, std::shared_ptr<Entity> player);
	// TODO: change place of radius

	// Proccess the state of the Entity at any time
	// during the game
	void Process(float deltaTime);

	void SetDead(bool dead);
	bool IsDead() const;

	// Physx data functions
	void InitActor(physx::PxRigidDynamic *actor, ActorData* data);
	physx::PxRigidDynamic* GetDynamic();
	physx::PxRigidActor* GetActor();
	void SetActor(physx::PxRigidActor* newActor);
	void SetDynamic(physx::PxRigidDynamic* newDynamic);
	int GetPlayerLives();
	void RemoveLife(float time);
	bool GetInvincibility();
	float SetCurrentlyInvincible(float time, float m_timeHit);
	void UpdateInvincibilityStatus(float time);
	bool IsCollided();
	void SetCollided(bool collide);
	void SetWinner(std::string player);
	std::string GetWinner();

	// Releases the resources safely that the entity holds
	void release();

	// Check if Dead according to life time and returns if entity
	// is dead
	boolean isDead(float lifeTime);

	// Set TOB time of birth
	void setTimeOfBirth(float TOB);
	

protected:

private:
	//Member Data:
public:
	// Max life time in seconds
	float MAX_LIFE_TIME = 1;
	// Current life time in seconds
	float m_lifeTime;
	// Name of entity
	std::string m_name;
	// Type of entity in string format
	std::string m_entityType;
	enum type {player, ball, rocket, box};
	int entityType;
	// Limit of balls the player can fire at one time
	int m_ballLimit = 3;
	
	// PhysX Actor Variables
	// Cannot create unique because object is protected
	physx::PxRigidActor* actor;
	// Stores initial data of actor
	std::unique_ptr<ActorData> actorData;
	//Stores Rigid Dynamics of Actor
	physx::PxRigidDynamic* dynamic;
	// Stores Matrix of entity
	DirectX::SimpleMath::Matrix matrix;
	// Stores the input from the entity
	std::shared_ptr<CapturedInputData> m_capturedInput;
	// Keyboard controls
	std::vector<DirectX::Keyboard::Keys> m_controls;




protected:
	// Default Physx variables
	float DENSITY = 1.0f; // How dense
	float ADAMPENING = 0.75f; // IDK
	// TODO: move this somewhere where it can be changed on init
	float RADIUS = 0.5;
	// Used to update the game state checked in the update function
	bool m_dead;

	// Model Data Pointer which contains information about the model
	// TODO: add pointer
	ModelData* m_modelData;

	//Life check
	float m_timeHit, newHitTime;
	float m_iFrameTime;
	float INVINCIBILITY_TIME = 1.f;
	float currentlyInvisible;
	bool isInvincible;
	int m_numLives;
	float COLLISION = 0.3f;
	bool collided;
	std::string winner;

private:

};

#endif //__ENTITY_H__
