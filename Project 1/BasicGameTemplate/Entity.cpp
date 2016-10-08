#include "pch.h"
#include "Entity.h"


Entity::Entity()
{
	
}

Entity::~Entity()
{
	
}

void
Entity::Init(std::string name, const char modelFileLoc,
float x, float y, float z, int type, float radius)
{
	// Set alive
	m_dead = false;

	// Set name
	m_name = name;

	// set type
	entityType = type;

	// Set entity location

	// Set model location

	// Set model object
	//m_modelData.SetModel(modelFileLoc);

	// Set Physx Actor object data accoring to their type
	if (type == box)
	{
		actorData->InitBoxActor(1, x, y, z, DENSITY, x, y, z, ADAMPENING, x, y, z);
	}

	if (type == ball)
	{
		actorData->InitSphereActor(0, RADIUS, DENSITY, x, y, z, ADAMPENING, x, y, z);
	}
	
	

	// TODO: check if m_modelData is not null else crash program

}

void
Entity::Process(float deltaTime)
{
	// is hit?

	//boundaries

	// update position variables of entity
	m_x += m_velocityX;
	m_y += m_velocityY;

	// set updated position of model
}

bool
Entity::IsCollidingWith(Entity& e)
{
	return (false);
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

/* Get position Functions */
float
	 Entity::GetPositionX() const
{
	return (m_x);
}

float
Entity::GetPositionY() const
{
	return (m_y);
}

float
Entity::GetPositionZ() const
{
	return (m_z);
}


/* Setting Position Functions */

void
Entity::SetPositionX(float x)
{
	m_x = x;
}

void
Entity::SetPositionY(float y)
{
	m_y = y;
}

void
Entity::SetPositionZ(float z)
{
	m_z = z;
}

/* Get Velocity Functions */

float
Entity::GetXVelocity() const
{
	return (m_velocityX);
}

float
Entity::GetYVelocity() const
{
	return (m_velocityY);
}

float
Entity::GetZVelocity() const
{
	return (m_velocityZ);
}

/* Set Velocity Functions */

void
Entity::SetXVelocity(float x)
{
	m_velocityX = x;
}

void
Entity::SetYVelocity(float y)
{
	m_velocityY = y;
}

void
Entity::SetZVelocity(float z)
{
	m_velocityZ = z;
}

void 
Entity::InitActor(physx::PxRigidDynamic *actor, ActorData* data)
{

	// Creating Sphere Geometry
	if (data->type == 0)
	{
		physx::PxSphereGeometry geometry(data->m_sphereGeometry);
		actor = PxCreateDynamic(*gPhysicsSDK, data->m_transform, data->m_sphereGeometry, *mMaterial, data->m_density);
	}
	if (data->type == 1)
	{
		PxBoxGeometry geometry(data->m_boxGeometry);
		actor = PxCreateDynamic(*gPhysicsSDK, data->m_transform, data->m_sphereGeometry, *mMaterial, data->m_density);
	}

	// Take all variables from Actor Data object and init actor

	actor->setAngularDamping(data->m_angularDampening);
	actor->setLinearVelocity(data->m_linearVelocityVector);
	if (!actor)
		cerr << "create actor failed!" << endl;
	gScene->addActor(*actor);
	// TODO: add to actor vector 
}

