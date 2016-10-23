/**
DogeyBalls
CapturedInputData.h
Purpose: Contains the data recieved from the input

@author Miguel Saavedra
@version 1.0 14/10/16
*/
#include "pch.h"
#include "PxPhysicsAPI.h"
using namespace DirectX;

// Captured Input Stored in object
class CapturedInputData{
	// Public member functions
public:
	CapturedInputData(float x, float y, float z);
	~CapturedInputData();

	// Setters from input
	void SetCameraVector(DirectX::SimpleMath::Vector3 vec);
	void SetEntityVector(physx::PxVec3 vec);
	void SetBallShotVector(physx::PxVec3 vec);
	void SetBallShotDisplacementVector(physx::PxVec3 vec);
	void SetBallShot(bool ball);
	void SetGameState(std::string state);
	void SetResetGame(bool reset);
	void SetBallCharge(float increaseChargeAmount);


	// Public members variables
public:
	// Stores value to be added to the cameras position
	SimpleMath::Vector3 cameraVector;
	// Stores value to be added to an entities position
	physx::PxVec3 entityVector;
	// Stores a boolean to store a flag to shoot ball
	bool m_ballShot;
	//Stores value of gamestate
	std::string m_gameState;
	// Stores a flag for the reset command
	bool m_resetGame;
	// Stores vector for which way the ball will be shot
	physx::PxVec3 ballVector;
	// Stores vector for which way the ball will be displaced
	physx::PxVec3 displaceVector;
	// Stores the float value of the power of the shot
	float m_ballCharge;
};