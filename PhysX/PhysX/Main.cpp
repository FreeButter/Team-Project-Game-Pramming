#include <stdio.h>
#include <PxPhysicsAPI.h>

using namespace std;
using namespace physx;

// link libraries
#pragma comment(lib, "PhysX3CHECKED_x86.lib")
#pragma comment(lib, "PhysX3CommonCHECKED_x86.lib")
#pragma comment(lib, "PhysX3ExtensionsCHECKED.lib")

// Physx Physics
PxPhysics *mPhysics = NULL;

void initPhysX() {
	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;
	PxFoundation *mFoundation = NULL;

	printf("initializing PhysX\n");

	printf("creating Foundation\n");
	// create foundation object with default error and allocator callbacks.
	mFoundation = PxCreateFoundation(
		PX_PHYSICS_VERSION,
		gDefaultAllocatorCallback,
		gDefaultErrorCallback);

	printf("creating Physics\n");
	// create Physics object with the created foundation and with a 'default' scale tolerance.
	mPhysics = PxCreatePhysics(
		PX_PHYSICS_VERSION,
		*mFoundation,
		PxTolerancesScale());

	printf("PhysX initialized\n");

	printf("shutting down\n");
	mPhysics->release();
	mFoundation->release();
}

int main() {
	initPhysX();

	return 0;
}