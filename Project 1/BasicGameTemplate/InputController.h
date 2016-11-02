/**
DogeyBalls
InputController.h
Purpose: Controls and routes commands from an IO device

@author Miguel Saavedra & Vivian Ngo
@version 1.0 13/10/16
*/
#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H
#include "pch.h"
#include <memory>
#include "DeviceResources.h"
#include "Entity.h"
#include "CapturedInputData.h"
#include "Camera.h"
using namespace DirectX;

class InputController
{

public: // Public Function Definitions
	InputController();
	~InputController();

	// Initialization and management
	void InitController(HWND window);
	
	// Catches the input state and key presses
	void UpdateCatchInput(std::shared_ptr<Entity> e, float elapsedTime,
		std::shared_ptr<Camera> camera, int gamestate);

	//void MovementKeyboard(Keyboard::State kb, std::shared_ptr<Entity> e);
	void MovementControlP1(GamePad::State state, Keyboard::State kb, std::shared_ptr<Entity> e);
	void MovementControlP2(GamePad::State state2, Keyboard::State kb, std::shared_ptr<Entity> e);

	// Updates camera position
	void UpdateCameraObject(std::shared_ptr<Camera> camera);

	//GameController Kill and Resume
	void ResumeActivateControls();
	void SuspendDeactivateControls();


	// TODO: Perhaps Change location of where the
	//the variables edited are located
	// Function: Takes the input from 
	//the user and changes values, which then 
	//can be called by other Controllers
	//InputController::capturedInputData UpdateCatchInput();

public: // Public Member Definitions

	// Member objects

	// Captured Input Stored in object
	struct capturedInputData {
		// Stores value to be added to an entities position
		SimpleMath::Vector3 cameraVector;
		physx::PxVec3 entityVector;
	};

	// Member Primitives

	// Member pointers
	std::unique_ptr<Keyboard> m_keyboard;
	std::unique_ptr<Mouse> m_mouse;

	// m_resetGame flag
	bool resetGame;

	// m_ballshot flag;
	bool m_ballshot_p1;
	bool m_ballshot_p2;

	// m_gameState flag
	std::string m_gameState;

	//TODO:
	// Passes these variable to PhysXController
	// to add force and move a dynamic
	// through the update catch input method
	float velX = 0.0f;
	float velY = 0.0f;
	float velZ = 0.0f;

	// Linear Velocity vector
	// Change balls direction of shot
	float linearBallX = 0.0f;
	float linearBallY = 0.0f;
	float linearBallZ = 0.0f;

	// Adjust the displacement of the ball
	// being shot
	float displaceX = 0.0f;
	float displaceY = 0.0f;
	float displaceZ = 0.0f;

	// Debug variables
	bool m_debugMode;
	bool m_keyboardMode;
	DirectX::SimpleMath::Vector3 m_cameraPos;
	float m_pitch;
	float m_yaw;
	float MAX_PITCH = 0.99;
	float MIN_PITCH = -0.99;

	// Init Debug move camera for init position
	bool initCamera = true;

	// trackers for input
	std::unique_ptr<Keyboard::KeyboardStateTracker> m_keyboardTracker;

	//GameController
	std::unique_ptr<DirectX::GamePad> m_gamePad;
	DirectX::GamePad::ButtonStateTracker m_buttons_p1;
	DirectX::GamePad::ButtonStateTracker m_buttons_p2;

	// velocity Variables
	float RIGHT_SHOOT = -30.0f;
	float DOWN_SHOOT = -30.0f;
	float LEFT_SHOOT = 30.0f;
	float UP_SHOOT = 30.0f;
	float CHARGE_UP = 0.7f;
	float BALL_DISPLACEMENT = 1.2;
};

#endif //__INPUTCONTROLLER_H__