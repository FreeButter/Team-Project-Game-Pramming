#include <malloc.h>
#include <PxAllocatorCallback.h>

class PxDefaultAllocator : public physx::PxAllocatorCallback
{
	void* allocate(size_t size, const char*, const char*, int)
	{
		return _aligned_malloc(size, 16);
	}

	void deallocate(void* ptr)
	{
		_aligned_free(ptr);
	}
};
