#include "pch.h"
#include "ActorData.h"


ActorData::ActorData()
{

}	

ActorData::~ActorData()
{
	
}

void
ActorData::InitBoxActor(int type, float dX, float dY, float dZ, float density, float tX, float tY, float tZ, float angularDampening, float vX, float vY, float vZ)
{
	m_type = type;
	// Set Up Cube
	m_density = 1.0f;
	m_transformVector = physx::PxVec3(tX, tY, tZ);
	m_transform = physx::PxTransform(m_transformVector, physx::PxQuat::createIdentity());
	float m_angularDampening = angularDampening;
	m_linearVelocityVector = physx::PxVec3(vX, 0, vZ);
	// Geometry
	dimensions = physx::PxVec3(0.5f, 0.5f, 0.5f);
	m_boxGeometry = physx::PxBoxGeometry(dimensions);
}

void ActorData::InitSphereActor(int type, float radius, float density, float tX, float tY, float tZ, float angularDampening, float vX, float vY, float vZ)
{
	m_type = type;
	// Set up Sphere
	m_density = density;
	m_transformVector = physx::PxVec3(tX, tY, tZ);
	m_transform = physx::PxTransform(m_transformVector, physx::PxQuat::createIdentity());
	float m_angularDampening = angularDampening;
	// Geometry
	m_linearVelocityVector = physx::PxVec3(vX, vY, vZ);
	m_sphereGeometry = physx::PxSphereGeometry(radius);
}
