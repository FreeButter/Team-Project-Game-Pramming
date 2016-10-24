#ifndef GAME_H
#define GAME_H
//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include <SimpleMath.h>
#include "ModelController.h"
#include "PhysXController.h"
#include "InputController.h"
#include "TextController.h"
#include "SoundController.h"
#include "Entity.h"
#include "Camera.h"



// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game : public DX::IDeviceNotify
{
public:

    Game();
	~Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();
    void Render();

    // Rendering helpers
    void Clear();

    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

	// Entity Game logic functions

	// Checks for collisions
	// Is given 2 entities and checks if entities collided
	void CheckCollision(std::shared_ptr<Entity> entity, std::shared_ptr<Entity> e);

	// Spawns a ball
	// Is given a player pointer and spawns a ball from
	// its location
	void SpawnBall(std::shared_ptr<Entity> player);

	// Releases the Entities resources and removes it safely from the
	// vector. The function takes an iterator to identify which Entity
	// to release. This will return back an iterator to revalidate the
	// Iterator in the outside scope
	std::vector<std::shared_ptr<Entity>>::iterator 
		ReleaseEntity(std::vector<std::shared_ptr<Entity>>::iterator EntityIt);


	// Releases the Entity Ball resources and removes it safely from the
	// vector. The function takes an iterator to identify which Entity
	// to release. This will return back an iterator to revalidate the
	// Iterator in the outside scope
	std::vector<std::shared_ptr<Entity>>::iterator
		ReleaseBall(std::vector<std::shared_ptr<Entity>>::iterator EntityIt);

	// Listens for a reset flag and Resets the game resources calling 
	// the initialize function. 
	void CheckResetGame(shared_ptr<Entity> entity);

	// Intializes the two players and adds them to their respective vector
	void InitEntities();

	// Intializes the two players in the physx scene
	void InitEntitiesPhysx();

	
	void Release();

	// Game States
	enum m_gameStates {
		MENU, PLAYING, PAUSED, LOST
	};
	int m_gameState = MENU;
private:

    void Update(DX::StepTimer const& timer);

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

	float m_time;
	int m_countdown = 60;

	// Controller Unique Pointers
	std::unique_ptr<ModelController> m_modelController;
	std::unique_ptr<PhysXController> m_physXController;
	std::unique_ptr<InputController> m_inputController;
	std::unique_ptr<TextController> m_textController;
	std::unique_ptr<SoundController> m_soundController;

	//Entities 
	std::vector<std::shared_ptr<Entity>> m_entityVector;
	std::vector<std::shared_ptr<Entity>>::iterator m_entityIt;

	// vector to store all the balls
	std::vector<std::shared_ptr<Entity>> m_ballVector;
	std::vector<std::shared_ptr<Entity>>::iterator m_ballIt;

	// Model Unique Pointers
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<CapturedInputData> m_capturedInputData;

	// Depended game Variables
	DirectX::SimpleMath::Vector3 CameraPosition = DirectX::SimpleMath::Vector3(0, 7, -15);
	DirectX::SimpleMath::Vector3 PlayerPosition = DirectX::SimpleMath::Vector3(0, 10, 0);
	
	// Window Dimensions
	// Used for UI element rendering
	float m_windowHeight;
	float m_windowWidth;

	//JSON parser
	float posX, posY, posZ;



	// FMOD Sounds
	SoundClass backgroundMusic;
	SoundClass hitSound;
	SoundClass ballShot;

	// DEBUG TEST COLLISION BOOLEAN
	bool collided = false;

	//colours
	DirectX::XMVECTORF32 color1;
	DirectX::XMVECTORF32 color2;

	//Winner
	std::string winnerStatement;

	float secondLoadoff;
};

#endif