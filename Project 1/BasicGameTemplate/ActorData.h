/**
DogeyBalls
ActorData.h
Purpose: Store data and manipulate the Actor objects
behaivour and state

@author Miguel Saavedra
@version 1.0 6/10/16
*/
#ifndef __ACTORDATA_H__
#define __ACTORDATA_H__
#include <geometry/PxBoxGeometry.h>
#include <geometry/PxSphereGeometry.h>
#include <PxTransform.h>


// Forward Declarations:
class Entity;

class ActorData
{
	//Member Methods:
public:
	ActorData();
	~ActorData();
	
	// init Sphere data
	void InitSphereActor(int type, float radius, float density, float tX, float tY, float tZ, float angularDampening, float vX, float vY, float vZ);
	// init Box data
	void InitBoxActor(int type, float dX, float dY, float dZ, float density, float tX, float tY, float tZ, float angularDampening, float vX, float vY, float vZ);

	

protected:

private:
	//Member Data:
public:
	// Density
	physx::PxReal m_density;
	// Tranform the size
	physx::PxVec3 m_transformVector;
	physx::PxTransform m_transform;
	// Dampening
	float m_angularDampening;
	// Velocity
	physx::PxVec3 m_linearVelocityVector;
	// Geometry of a Cube
	physx::PxBoxGeometry m_boxGeometry;
	// Dimensions of a shape other than spheres
	physx::PxVec3 dimensions;
	// Geometry of a Sphere
	physx::PxSphereGeometry m_sphereGeometry;
	// Type of object
	int type;



protected:

private:


};

#endif //__ACTORDATA_H__
