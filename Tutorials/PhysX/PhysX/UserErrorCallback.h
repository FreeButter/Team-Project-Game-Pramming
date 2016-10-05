#include <extensions/PxDefaultErrorCallback.h>
#include <cstdio>

class UserErrorCallback : public physx::PxErrorCallback
{
public:
	virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file,
		int line)
	{
		// error processing implementation
		printf("you fucked up");
	}
};
