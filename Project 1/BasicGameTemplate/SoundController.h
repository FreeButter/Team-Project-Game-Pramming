// COMP710 GP 2D Framework
#ifndef __SOUNDSYSTEM_H__
#define __SOUNDSYSTEM_H__

#include "pch.h"
#include "fmod.hpp"

typedef FMOD::Sound* SoundClass;

class SoundController
{
public:
	// Pointer to the FMOD instance
	FMOD::System *m_pSystem;
	SoundController();

	// Class methods
	void createSound(SoundClass *pSound, const char* pFile);
	void playSound(SoundClass pSound, bool bLoop);
	void releaseSound(SoundClass pSound);
};

#endif // __SOUNDSYSTEM_H__