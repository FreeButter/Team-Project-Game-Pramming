/**
DogeyBalls
CapturedInputData.cpp
Purpose: Contains the data recieved from the input

@author Miguel Saavedra
@version 1.0 14/10/16
*/

#include "pch.h"
#include "CapturedInputData.h"

CapturedInputData::CapturedInputData(float x, float y, float z) 
{
	
	entityVector = physx::PxVec3(x, y, z);
	displaceVector = physx::PxVec3(0, 0, 0);
	ballVector = physx::PxVec3(0, 0, 0);
	cameraVector = DirectX::SimpleMath::Vector3::Zero;
	m_gameState = "MENU";
	m_resetGame = false;
	m_exitGame = false;
	m_ballCharge = 0;
}

CapturedInputData::~CapturedInputData()
{

}

void
CapturedInputData::SetCameraVector(DirectX::SimpleMath::Vector3 vec)
{
	cameraVector = vec;
}

void
CapturedInputData::SetEntityVector(physx::PxVec3 vec)
{
	entityVector = vec;
}

void 
CapturedInputData::SetBallShotVector(physx::PxVec3 vec)
{
	ballVector = vec;
}

void 
CapturedInputData::SetBallShotDisplacementVector(physx::PxVec3 vec)
{
	displaceVector = vec;
}

void 
CapturedInputData::SetBallShot(bool ball)
{
	m_ballShot = ball;
}

void
CapturedInputData::SetGameState(std::string state)
{
	m_gameState = state;
}

void 
CapturedInputData::SetResetGame(bool reset)
{
	if (reset == true)
		m_resetGame = reset;
}

void 
CapturedInputData::SetBallCharge(float increaseChargeAmount)
{
	m_ballCharge = increaseChargeAmount;
}

void
CapturedInputData::SetExitGame(bool exit)
{
	m_exitGame = exit;
}
