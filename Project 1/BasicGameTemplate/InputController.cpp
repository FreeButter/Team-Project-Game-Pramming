/*
DogeyBalls
InputController.cpp
Purpose: Controls and routes commands from an IO device

@author Everyone
@version 1.0 13/10/16
*/
#include "pch.h"
#include "InputController.h"
#include "Game.h"


InputController::InputController()
{
	m_debugMode = false;
	m_keyboardMode = false;
	m_gameState = "MENU";
}

InputController::~InputController()
{

}

void
InputController::InitController(HWND window)
{
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(window);
	m_keyboardTracker = std::unique_ptr<Keyboard::KeyboardStateTracker>(new Keyboard::KeyboardStateTracker);
	m_gamePad = std::make_unique<GamePad>();
}

void
InputController::UpdateCatchInput(std::shared_ptr<Entity> e,
float elapsedTime, std::shared_ptr<Camera> camera, int gamestate)
{

	// Gamestate
	if (gamestate == 0)
	{
		// menu
		m_gameState = "MENU";	
	}
	if (gamestate == 1)
	{
		// playing
		m_gameState = "PLAYING";
	}
	if (gamestate == 2)
	{
		// paused
		m_gameState = "PAUSED";
	}
	if (gamestate == 3)
	{
		// lost
		m_gameState = "LOST";
	}

	// Set Variables Here
	SimpleMath::Vector3 move = SimpleMath::Vector3::Zero;
	resetGame = false;
	
	// init tracker for mouse and keyboard
	/*std::unique_ptr<Keyboard::KeyboardStateTracker> tracker(new Keyboard::KeyboardStateTracker);*/
	auto kb = m_keyboard->GetState();
	m_keyboardTracker->Update(kb);

	GamePad::State state = m_gamePad->GetState(0);
	GamePad::State state2 = m_gamePad->GetState(1);
	

	// Bandaided camera init position
	// TODO: Give 
	m_yaw = camera->GetYaw();
	m_pitch = camera->m_pitch;
	m_cameraPos = camera->getPosition();

	
	if (gamestate == Game::m_gameStates::MENU)
	{
		if (m_keyboardTracker->IsKeyPressed(Keyboard::Space))
		{
			e->m_capturedInput->SetGameState("PLAYING");
			
		}
		if (state.IsAPressed() || state2.IsAPressed()){
			e->m_capturedInput->SetGameState("PLAYING");
			m_buttons_p1.Update(state);
			m_buttons_p2.Update(state);
		}
		if (state2.IsViewPressed() || state.IsViewPressed())
		{
			e->m_capturedInput->SetExitGame(true);
		}
	}
	if (m_gameState == "LOST")
	{
		m_gameState = "LOST";
		e->m_capturedInput->SetGameState(m_gameState);

		//Set vibration if lost
		if (e->GetWinner() == "player1")
		{
			m_gamePad->SetVibration(1, 5.f, 5.f);
		}
		else if (e->GetWinner() == "player2")
		{
			m_gamePad->SetVibration(0, 5.f, 5.f);
		}
		else if (e->GetWinner() == "tie")
		{
			m_gamePad->SetVibration(0, 5.f, 5.f);
			m_gamePad->SetVibration(1, 5.f, 5.f);
		}

		//Set continue game if Y pressed
		if (state.IsYPressed() || state2.IsYPressed() ||kb.IsKeyDown(Keyboard::Y))
		{
			m_gamePad->SetVibration(0, 0.f, 0.f);
			m_gamePad->SetVibration(1, 0.f, 0.f);
			resetGame = true;
			//e->m_capturedInput->SetGameState("MENU");
			e->m_capturedInput->SetResetGame(resetGame);
			m_buttons_p1.Update(state);
			m_buttons_p2.Update(state);
		}
		if (state2.IsViewPressed() || state.IsViewPressed())
		{
			e->m_capturedInput->SetExitGame(true);
		}
	}
	if (m_gameState == "PLAYING")
	{
		// Toggle Debug mode
		if (m_keyboardTracker->IsKeyPressed(Keyboard::Home))
		{
			m_debugMode = !m_debugMode;
		}

		if (m_keyboardTracker->IsKeyPressed(Keyboard::P))
		{
			m_keyboardMode = !m_keyboardMode;
		}

		// Debug mode controls
		if (m_debugMode) {
			if (kb.R)
				resetGame = true;

			if (kb.Escape)
				e->m_capturedInput->SetExitGame(true);

			if (kb.Up)
				move.y += 1.f;

			if (kb.Down)
				move.y -= 1.f;

			if (kb.W)
				move.z += 1.f;

			if (kb.S)
				move.z -= 1.f;

			if (kb.Left)
				m_yaw += tanf(elapsedTime)*1.0f;

			if (kb.Right)
				m_yaw -= tanf(elapsedTime)*1.0f;

			if (kb.A)
				move.x += 1.f;

			if (kb.D)
				move.x -= 1.f;

			if (kb.Q){
				if (m_pitch > MIN_PITCH)
					m_pitch -= tanf(elapsedTime)*1.0f;
			}

			if (kb.E){
				if (m_pitch < MAX_PITCH)
					m_pitch += tanf(elapsedTime)*1.0f;
			}
		}

		// update Camera pos variable
		SimpleMath::Quaternion q = SimpleMath::Quaternion::
			CreateFromYawPitchRoll(m_yaw, m_pitch, 0.f);
		move = SimpleMath::Vector3::Transform(move, q);
		move *= camera->MOVEMENT_GAIN;

		m_cameraPos += move;

		// Keyboard Movement DEBUG - MUST COMMENT OUT BOTH PLAYER MOVEMENT METHODS TO USE
		if (m_keyboardMode){
			//MovementKeyboard(kb, e);
			m_ballshot_p1 = false;
			m_ballshot_p2 = false;
		}else{
		// Delligate update logic for each player
			if (e->m_name == "player1")
				MovementControlP1(state, kb, e);
			else
				MovementControlP1(state, kb, e);
			//MovementControlP2(state2, kb, e);

			if (e->m_name == "player1"){
				if (e->IsCollided() == true)
				{
					m_gamePad->SetVibration(0, 10.f, 10.f);
				}
				else{
					if (e->IsCollided() == false){
						m_gamePad->SetVibration(1, 0, 0);
					}
				}
			}
			else if (e->m_name == "player2"){
				if (e->IsCollided() == true)
				{
					m_gamePad->SetVibration(1, 10.f, 10.f);
				}
				else{
					if (e->IsCollided() == false){
						m_gamePad->SetVibration(0, 0, 0);
					}
				}
			}
		}

		// Set pitch and yaw on camera object
		camera->SetPitch(m_pitch);
		camera->SetYaw(m_yaw);
		camera->SetPosition(m_cameraPos);
		if (state.IsViewPressed() || state2.IsViewPressed()){

			PostQuitMessage(0);
		}
		e->m_capturedInput->SetResetGame(resetGame);
	}
	
}

/**
* Keyboard Movement used to control both players at once
*/
//void InputController::MovementKeyboard(Keyboard::State kb, std::shared_ptr<Entity> e)
//{
//	if (kb.IsKeyDown(Keyboard::D))
//	{
//		velX = -1.5f;
//	}
//		else if (kb.IsKeyUp(Keyboard::D) || kb.IsKeyUp(Keyboard::A))
//		{
//			velX = 0.0f;
//		}
//	if (kb.IsKeyDown(Keyboard::A))
//	{
//		velX = 1.5f;
//	}
//
//	if (kb.IsKeyDown(Keyboard::W))
//	{
//		velZ = 1.5f;
//	}
//		else if (kb.IsKeyUp(Keyboard::W) || kb.IsKeyUp(Keyboard::A))
//		{
//			velZ = 0.0f;
//		}
//
//	if (kb.IsKeyDown(Keyboard::S))
//	{
//		velZ = -1.5f;
//	}
//
//	if (kb.IsKeyDown(Keyboard::Space) && velY <= 0.0f)
//	{
//		velY = 1.5f;
//	}
//		else if (kb.IsKeyUp(Keyboard::Space))
//		{
//			velY = 0.0f;
//		}
//
//	if (m_keyboardTracker->IsKeyPressed(Keyboard::Y))
//	{
//		m_ballshot_p1 = true;
//		m_ballshot_p2 = true;
//	}
//	if (m_keyboardTracker->IsKeyReleased(Keyboard::Y))
//	{
//		m_ballshot_p1 = false;
//		m_ballshot_p2 = false;
//	}
//
//	physx::PxVec3 entityVector = physx::PxVec3(velX, velY, velZ);
//	
//	// Set Values in the capturedInputData Object for the entity
//	if (e->m_name == "player1")
//	{
//		e->m_capturedInput->SetEntityVector(entityVector);
//		e->m_capturedInput->SetBallShot(m_ballshot_p2);
//		e->m_capturedInput->SetBallShotVector(physx::PxVec3(30,0,0));
//		e->m_capturedInput->SetBallShotDisplacementVector(physx::PxVec3(2, 0, 0));
//		//e->m_capturedInput->SetBallShot(m_ballshot_p2);
//	}
//}

void
InputController::MovementControlP1(GamePad::State state, Keyboard::State kb, std::shared_ptr<Entity> e)
{
		// Control variables
		float velX = 0;
		float velY = 0;
		float velZ = 0;
		// Chooses which controller to vibrate
		int controller = 0;
		if (state.IsViewPressed()) //|| controller2.IsViewPressed())
		{
			e->m_capturedInput->SetExitGame(true);
		}

		if (state.IsLeftShoulderPressed())
		{
			m_gamePad->SetVibration(controller, 0.25f, 0.25f);
		}
		if (state.IsRightShoulderPressed())
		{
			m_gamePad->SetVibration(controller, 0, 0);
		}

		if (m_buttons_p1.rightTrigger == GamePad::ButtonStateTracker::PRESSED || m_keyboardTracker->IsKeyReleased(Keyboard::B))
		{
			m_ballshot_p1 = true;
			if (state.thumbSticks.rightX >= -0.2 && state.thumbSticks.rightX <= 0.2 && state.thumbSticks.rightY >= -0.2 && state.thumbSticks.rightY <= 0.2)
			{
				m_ballshot_p1 = false;
			}
			//e->m_capturedInput->SetBallCharge(0);
			// get Angle from right thumb stick
			float xStick = state.thumbSticks.rightX;
			float yStick = state.thumbSticks.rightY;

			// Set linear velocity by angle
			linearBallZ = (DOWN_SHOOT*-yStick);
			linearBallX = (RIGHT_SHOOT*xStick);

			// Debug
			m_ballshot_p1 = true;
		}

		// Controls Z Movement
		// Currently uses vector with directx 11 key mapping
		if (state.IsDPadDownPressed() || state.IsLeftThumbStickDown() || kb.IsKeyDown(e->m_controls[0]))
		{
			velZ = -0.7f;
		}
		else
		{
			velZ = 0.0f;
		}
		if (state.IsDPadUpPressed() || state.IsLeftThumbStickUp() || kb.IsKeyDown(Keyboard::S))
		{
			velZ = +0.7f;
		}

		// Controls Horizontal Movement
		if (state.IsDPadLeftPressed() || state.IsLeftThumbStickLeft() || kb.IsKeyDown(Keyboard::A))
		{
			velX = 0.7f;
		}
		else
		{
			velX = 0.0f;
		}
		if (state.IsDPadRightPressed() || state.IsLeftThumbStickRight() || kb.IsKeyDown(Keyboard::D))
		{
			velX = -0.7f;
		}

		// Does Jumping
		if (state.IsAPressed() && e->GetDynamic()->getGlobalPose().p.y <= 0.7f || kb.IsKeyDown(Keyboard::Space) && e->GetDynamic()->getGlobalPose().p.y <= 0.7f)
		{
			velY = 4.5f;
		}
		else{
			velY = 0;
		}

		// Ball Shooting
		// Down Stick
		if (state.IsRightThumbStickDown())
		{
			displaceZ = -BALL_DISPLACEMENT;
		}
		// Right Stick
		if (state.IsRightThumbStickRight())
		{
			displaceX = -BALL_DISPLACEMENT;
		}
		// Left Stick
		if (state.IsRightThumbStickLeft())
		{
			displaceX = BALL_DISPLACEMENT;
		}
		// Up Stick
		if (state.IsRightThumbStickUp())
		{
			displaceZ = BALL_DISPLACEMENT;
		}


		m_buttons_p1.Update(state);
		// Create vector to store movement of the entity
		physx::PxVec3 entityVector = physx::PxVec3(velX, velY, velZ);
		physx::PxVec3 ballVector = physx::PxVec3(linearBallX, 0, linearBallZ);
		physx::PxVec3 ballDisplacementVector = physx::PxVec3(displaceX, 0, displaceZ);
		displaceX = 0;
		displaceZ = 0;
		linearBallX = 0;
		linearBallZ = 0;

		// Set Values in the capturedInputData Object for the entity
		e->m_capturedInput->SetEntityVector(entityVector);
		e->m_capturedInput->SetBallShot(m_ballshot_p1);
		e->m_capturedInput->SetBallShotVector(ballVector);
		e->m_capturedInput->SetBallShotDisplacementVector(ballDisplacementVector);
		m_ballshot_p1 = false;
	
}

void 
InputController::MovementControlP2(GamePad::State state2, Keyboard::State kb, std::shared_ptr<Entity> e)
{
		int controller = 1;
		//auto controller2 = m_gamePad->GetState(1);
			if (state2.IsViewPressed()) //|| controller2.IsViewPressed())
			{
				e->m_capturedInput->SetExitGame(true);
			}
			if (m_buttons_p2.leftShoulder)
			{
				m_gamePad->SetVibration(controller, 0.25f, 0.25f);
			}
			if (m_buttons_p2.rightShoulder)
			{
				m_gamePad->SetVibration(controller, 0, 0);
			}

			// TODO: TODO: TODO: figure out how to get tracker for keyboard to shoot the ball
 			if (m_buttons_p2.rightTrigger == GamePad::ButtonStateTracker::PRESSED || m_keyboardTracker->IsKeyReleased(Keyboard::N))
			{
				m_ballshot_p2 = true;
				if (state2.thumbSticks.rightX >= -0.2 && state2.thumbSticks.rightX <= 0.2 && state2.thumbSticks.rightY >= -0.2 && state2.thumbSticks.rightY <= 0.2)
				{
					m_ballshot_p2 = false;
				}
				// Debug
				m_ballshot_p2 = true;

			}
			if (state2.IsDPadDownPressed() || state2.IsLeftThumbStickDown() || kb.IsKeyDown(Keyboard::Down))
			{
				velZ = -0.7f;
			}
			else{
				velZ = 0.0f;
			}
			if (state2.IsDPadUpPressed() || state2.IsLeftThumbStickUp() || kb.IsKeyDown(Keyboard::Up))
			{
				velZ = +0.7f;
			}
			if (state2.IsDPadLeftPressed() || state2.IsLeftThumbStickLeft() || kb.IsKeyDown(Keyboard::Left))
			{
				velX = 0.7f;
			}
			else{
				velX = 0.0f;
			}
			if (state2.IsDPadRightPressed() || state2.IsLeftThumbStickRight() || kb.IsKeyDown(Keyboard::Right))
			{
				velX = -0.7f;
			}
		
			// Does 2nd Place Jumping
			if (state2.IsAPressed() && e->GetDynamic()->getGlobalPose().p.y <= 0.7f || kb.IsKeyDown(Keyboard::RightControl) && e->GetDynamic()->getGlobalPose().p.y <= 0.7f)
			{
				velY = 4.5f;
			}
			else{
				velY = 0;
			}

			// get Angle from right thumb stick
			float xStick = state2.thumbSticks.rightX;
			float yStick = state2.thumbSticks.rightY;

			// Set linear velocity by angle
			linearBallZ = DOWN_SHOOT*-yStick;
			linearBallX = RIGHT_SHOOT*xStick;


			// Ball Shooting
			// Down Stick
			if (state2.IsRightThumbStickDown())
			{
				displaceZ = -BALL_DISPLACEMENT;
			}
			// Right Stick
			if (state2.IsRightThumbStickRight())
			{
				displaceX = -BALL_DISPLACEMENT;
			}
			// Left Stick
			if (state2.IsRightThumbStickLeft())
			{
				displaceX = BALL_DISPLACEMENT;
			}
			// Up Stick
			if (state2.IsRightThumbStickUp())
			{
				displaceZ = BALL_DISPLACEMENT;
			}

			m_buttons_p2.Update(state2);
			// Create vector to store movement of the entity
			physx::PxVec3 entityVector = physx::PxVec3(velX, velY, velZ);
			physx::PxVec3 ballVector = physx::PxVec3(linearBallX, 0, linearBallZ);
			physx::PxVec3 ballDisplacementVector = physx::PxVec3(displaceX, 0, displaceZ);

			
				// Set Values in the capturedInputData Object for the entity
				e->m_capturedInput->SetEntityVector(entityVector);
				e->m_capturedInput->SetBallShot(m_ballshot_p2);
				e->m_capturedInput->SetBallShotVector(ballVector);
				e->m_capturedInput->SetBallShotDisplacementVector(ballDisplacementVector);
				m_ballshot_p2 = false;
			

}

void
InputController::UpdateCameraObject(std::shared_ptr<Camera> camera)
{
	// Get Pitch and Yaw and Pos from Camera Object
	m_pitch = camera->GetPitch();
	m_yaw = camera->GetYaw();
	m_cameraPos = camera->m_cameraPos;

	float y = sinf(m_pitch);
	float r = cosf(m_pitch);
	float z = r*cosf(m_yaw);
	float x = r*sinf(m_yaw);

	XMVECTOR lookAt = m_cameraPos + SimpleMath::Vector3(x, y, z);
	XMMATRIX view = XMMatrixLookAtRH(m_cameraPos, lookAt, SimpleMath::Vector3::Up);

	camera->SetPosition(m_cameraPos);
	camera->SetView(view);
}


void
InputController::ResumeActivateControls()
{
	m_gamePad->Resume();
	m_buttons_p1.Reset();
	m_buttons_p2.Reset();
}

void
InputController::SuspendDeactivateControls()
{
	m_gamePad->Suspend();
}