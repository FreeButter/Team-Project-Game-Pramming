#include "pch.h"
#include "Entity.h"
#include "CapturedInputData.h"
#include "InputMapper.h"


Entity::Entity()
{

}

Entity::~Entity()
{
	
}

void
Entity::Init(const Value& json)
{
	// Input Mapper Object
	InputMapper input = InputMapper();

	// Set alive
	m_dead = json["entity_data"]["is_dead"].GetBool();

	// Set name
	m_name = json["entity_data"]["name"].GetString();

	//Set Lives
	m_numLives = json["entity_data"]["lives"].GetInt();

	m_timeHit = 0;
	collided = false;
	isInvincible = false;
	winner = "";

	// set Entity Type
	std::string type = json["entity_data"]["type"].GetString();
	if (type == "player")
		entityType = player;
	if (type == "ball")
		entityType = ball;
	if (type == "box")
		entityType = box;

	// Physx variable initialization
	// Position
	float positionX = json["physx_data"]["position"][0].GetFloat();
	float positionY = json["physx_data"]["position"][1].GetFloat();
	float positionZ = json["physx_data"]["position"][2].GetFloat();

	// Velocity
	float velocityX = json["physx_data"]["linear_velocity"][0].GetFloat();
	float velocityY = json["physx_data"]["linear_velocity"][1].GetFloat();
	float velocityZ = json["physx_data"]["linear_velocity"][2].GetFloat();

	actorData = std::unique_ptr<ActorData>(new ActorData());
	// Set Physx Actor object data accoring to their type
	if (entityType == box)
	{
		float dimensionX = json["physx_data"]["dimensions"][0].GetFloat();
		float dimensionY = json["physx_data"]["dimensions"][1].GetFloat();
		float dimensionZ = json["physx_data"]["dimensions"][2].GetFloat();
		actorData->InitBoxActor(Entity::box, dimensionX, dimensionY, dimensionZ, DENSITY, positionX, positionY, positionZ, ADAMPENING, velocityX, velocityY, velocityZ);
	}

	if (entityType == ball)
	{
		actorData->InitSphereActor(Entity::ball, RADIUS, DENSITY, positionX, positionY, positionZ, ADAMPENING, velocityX, velocityY, velocityZ);
	}
	
	// Set actor and dynamic to nulls
	actor = 0;
	
	// Set input data to null
	m_capturedInput = std::shared_ptr<CapturedInputData>(new CapturedInputData(0, 0, 0));

	// Setup control scheme
	m_controls = std::vector<DirectX::Keyboard::Keys>();

	m_controls = input.MapKeys(json);
}

void
Entity::InitBall(const Value& json, std::shared_ptr<Entity> player)
{
	// Set alive
	m_dead = json["entity_data"]["is_dead"].GetBool();

	// Set name
	m_name = json["entity_data"]["name"].GetString();

	// set Entity Type
	std::string type = json["entity_data"]["type"].GetString();
	if (type == "ball")
		entityType = ball;

	float positionX = player->GetDynamic()->getGlobalPose().p.x + player->m_capturedInput->displaceVector.x;
	float positionY = player->GetDynamic()->getGlobalPose().p.y + player->m_capturedInput->displaceVector.y;
	float positionZ = player->GetDynamic()->getGlobalPose().p.z + player->m_capturedInput->displaceVector.z;

	float velocityX = player->m_capturedInput->ballVector.x;
	float velocityY = player->m_capturedInput->ballVector.y;
	float velocityZ = player->m_capturedInput->ballVector.z;

	actorData = std::unique_ptr<ActorData>(new ActorData());
	// Set Physx Actor object data accoring to their type
	if (entityType == ball)
	{
		actorData->InitSphereActor(Entity::ball, RADIUS, DENSITY, positionX, positionY, positionZ, ADAMPENING, velocityX, velocityY, velocityZ);
	}

	// Set actor and dynamic to nulls
	actor = 0;

	// Set input data to null
	m_capturedInput = std::shared_ptr<CapturedInputData>(new CapturedInputData(0, 0, 0));

}

void
Entity::SetDead(bool dead)
{
	m_dead = dead;
}

bool Entity::IsDead() const
{
	return m_dead;
}


physx::PxRigidDynamic* 
Entity::GetDynamic()
{
	return dynamic;
}

physx::PxRigidActor* 
Entity::GetActor()
{
	return actor;
}

void 
Entity::SetActor(physx::PxRigidActor* newActor)
{
	actor = newActor;
}

void 
Entity::SetDynamic(physx::PxRigidDynamic* newDynamic)
{
	dynamic = newDynamic;
}

void
Entity::release()
{
	// Release Physx Members
	dynamic->release();
}

bool
Entity::isExpired(float lifeTime)
{
	// Check life time is over
	if ((lifeTime - m_lifeTime) >= MAX_LIFE_TIME)
	{
		m_dead = true;
	}
	
	// Return if is dead
	return m_dead;
}

// Set TOB time of birth from elapsed seconds
void 
Entity::setTimeOfBirth(float TOB)
{
	m_lifeTime = TOB;
}

int 
Entity::GetPlayerLives()
{
	return m_numLives;
}

void 
Entity::RemoveLife(float time)
{
	// If hit time is 0
	if (m_timeHit == 0) {
		m_timeHit = time;
		// Damage player
		m_numLives -= 1;
		isInvincible = true;
		collided = true;
	}
}

void 
Entity::UpdateInvincibilityStatus(float time)
{
	// If recently been hit check when
	if ((time - m_timeHit > INVINCIBILITY_TIME))
	{
		// Set to normal
		isInvincible = false;
		// reset time hit
		m_timeHit = 0;
	}

	if ((time - m_timeHit) > COLLISION){
		collided = false;
	}
} 

bool 
Entity::GetInvincibility()
{
	return isInvincible;
}

bool 
Entity::IsCollided()
{
	return collided;
}

void
Entity::SetCollided(bool collide)
{
	collided = collide;
}

void 
Entity::SetWinner(std::string player)
{
	winner = player;
}

std::string
Entity::GetWinner()
{
	return winner;
}