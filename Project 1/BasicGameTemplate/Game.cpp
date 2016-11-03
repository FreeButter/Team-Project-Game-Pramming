//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "PhysXController.h"
#include "Entity.h"
#include "Camera.h"
#include <iostream>

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game()
{
	m_deviceResources = std::make_unique<DX::DeviceResources>();
	// Create Controllers unique pointers
	m_modelController = std::unique_ptr<ModelController>(new ModelController());
	m_physXController = std::unique_ptr<PhysXController>(new PhysXController());
	m_inputController = std::unique_ptr<InputController>(new InputController());
	m_textController = std::unique_ptr<TextController>(new TextController());
	m_soundController = std::unique_ptr<SoundController>(new SoundController());

	// Camera Model
	m_camera = std::shared_ptr<Camera>(new Camera());
	// User Input Model
	m_capturedInputData = std::unique_ptr<CapturedInputData>(new CapturedInputData(0, 0, 0));
	
	// Register Device Resources for rendering
    m_deviceResources->RegisterDeviceNotify(this);

	// Initialize Players
	InitEntities();

	// Init Physx Controller
	m_physXController->InitPhysX();
	
	secondLoadoff = 0;

}

Game::~Game()
{
	//m_entityIt = m_entityVector.begin();
	//ReleaseEntity(m_entityIt);
	//m_entityIt = m_entityVector.begin();
	//ReleaseEntity(m_entityIt);
	vector<std::shared_ptr<Entity>>().swap(m_entityVector);

}

void Game::InitEntities()
{
	// Load in a Json data for the Entity
	Parser::GetInstance().loadInFile("data.json");
	// Create two players and get set data from the json data
	const Value& jsonplayer1 = Parser::GetInstance().document["entity"];
	const Value& jsonplayer2 = Parser::GetInstance().document["entity2"];

	// Add player 1 to the Game 
	std::shared_ptr<Entity> player = std::shared_ptr<Entity>(new Entity());
	player->Init(jsonplayer1);
	m_entityVector.push_back(player);

	// Add player 2 to the Game 
	std::shared_ptr<Entity> player2 = std::shared_ptr<Entity>(new Entity());
	player2->Init(jsonplayer2);
	m_entityVector.push_back(player2);
}


void Game::InitEntitiesPhysx()
{
	// Init Entities into the Physx scene
	for (m_entityIt = m_entityVector.begin(); m_entityIt != m_entityVector.end(); ++m_entityIt)
	{
		// Init Entities Within the game
		std::shared_ptr<Entity> e = *m_entityIt;
		e->SetDynamic(m_physXController->InitActor(e));
	}
}


// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	// Set Dimensions of width and height
	m_windowWidth = width;
	m_windowHeight = height;

	// Init dependent resources
	m_deviceResources->SetWindow(window, width, height);

	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	// Creating the sounds for game
	m_soundController->createSound(&backgroundMusic, "assets\\8-bit-world.mp3");
	m_soundController->createSound(&hitSound, "assets\\explosion.wav");
	m_soundController->createSound(&ballShot, "assets\\BallShot.wav");

	// Plays background music
	m_soundController->playSound(backgroundMusic, true);

	// Init Controller input
	m_inputController->InitController(window);

	// Init camera
	m_camera->InitCamera(width, height, CameraPosition);

	//colors
	color1 = DirectX::Colors::White;
	color2 = DirectX::Colors::White;

	InitEntitiesPhysx();
}

void Game::CheckResetGame(shared_ptr<Entity> entity)
{
	if (entity->m_capturedInput->m_resetGame) {
		// Removal code
		// Remove Players
		m_entityIt = m_entityVector.begin();
		ReleaseEntity(m_entityIt);
		m_entityIt = m_entityVector.begin();
		ReleaseEntity(m_entityIt);


		// Re Init code1
		// Players
		InitEntities();
		InitEntitiesPhysx();

		// Reset Time
		m_countdown = 60;
		m_gameState = m_gameStates::MENU;
		
		shared_ptr<Entity> entity1 = *m_entityVector.begin();
		shared_ptr<Entity> entity2 = *(m_entityVector.begin()+1);
		entity1->m_capturedInput->SetGameState("MENU");
		entity2->m_capturedInput->SetGameState("MENU");
	}
}


#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

//  Function : Game Logic
void Game::Update(DX::StepTimer const& timer)
{

	// Get Seconds
	float elapsedTime = float(timer.GetElapsedSeconds());
	float time = float(timer.GetTotalSeconds());
	// Set Game time
	m_time = (float)timer.GetTotalSeconds();

	// playing
	for (m_entityIt = m_entityVector.begin(); m_entityIt != m_entityVector.end(); ++m_entityIt)
	{
		// Get Entity Pointer from iterator
		std::shared_ptr<Entity> e = *m_entityIt;
		
		if (e->m_capturedInput->m_exitGame)
		{
			Release();
			PostQuitMessage(0);
		}
	}


	// Check Result of gam on time
	if (m_countdown < 0)
		m_gameState = m_gameStates::LOST;

	if (m_gameState == m_gameStates::LOST)
	{
		// Get the two players
		std::shared_ptr<Entity> e = m_entityVector[0];
		std::shared_ptr<Entity> e2 = m_entityVector[1];
		// Check for each persons input
		CheckResetGame(e2);
		CheckResetGame(e);

		// Update Input Data Object
		m_inputController->UpdateCatchInput(e, elapsedTime, m_camera, m_gameState);
		m_inputController->UpdateCatchInput(e2, elapsedTime, m_camera, m_gameState);
	}

		if (m_gameState == m_gameStates::MENU)
		{
			// playing
			for (m_entityIt = m_entityVector.begin(); m_entityIt != m_entityVector.end(); ++m_entityIt)
			{
				// Get Entity Pointer from iterator
				std::shared_ptr<Entity> e = *m_entityIt;

				// Update Input Data Object
				m_inputController->UpdateCatchInput(e, elapsedTime, m_camera, m_gameState);

				// Put catched input into physx controller and update the entities position in the world
				m_physXController->StepPhysX(time, e->m_capturedInput->entityVector, e->GetDynamic());

				// Update the models
				m_modelController->Update(e);

				if (e->m_capturedInput->m_gameState == "PLAYING")
				{
					m_gameState = m_gameStates::PLAYING;
				}
			}
		}

		// If gamestate is playing
		if (m_gameState == m_gameStates::PLAYING)
		{
			// Listen for Reset Command From both players


			m_entityIt = m_entityVector.begin();
			// Get Entity Pointer from iterator
			if (m_entityVector.size() != 0)
			{
				std::shared_ptr<Entity> e = *m_entityIt;
				CheckResetGame(e);
			}


			// Loop through all entities and update them
			for (m_entityIt = m_entityVector.begin(); m_entityIt != m_entityVector.end(); ++m_entityIt)
			{
				// Get Entity Pointer from iterator
				std::shared_ptr<Entity> e = *m_entityIt;

				// Update Input Data Object
				m_inputController->UpdateCatchInput(e, elapsedTime, m_camera, m_gameState);

				// Put catched input into physx controller and update the entities position in the world
				m_physXController->StepPhysX(time, e->m_capturedInput->entityVector, e->GetDynamic());

				// Update the models
				m_modelController->Update(e);

				if (e->m_capturedInput->m_ballShot == true)
				{
					if (m_ballVector.size() < e->m_ballLimit)
					{
						SpawnBall(e);
					}
					e->m_capturedInput->m_ballShot = false;
				}
			}

			// Updates and removes Dodgeball 
			for (m_ballIt = m_ballVector.begin(); m_ballIt != m_ballVector.end(); ++m_ballIt)
			{
				// Init Entities Within the game
				std::shared_ptr<Entity> ball = *m_ballIt;

				if (ball->isExpired(m_time))
				{
					// Release the ball
					m_ballIt = ReleaseBall(m_ballIt);
				}

				if (!m_ballVector.size() > 0)
				{
					break;
				}

				// Check For Collision With balls
				for (m_entityIt = m_entityVector.begin(); m_entityIt != m_entityVector.end(); ++m_entityIt)
				{

					// Get Entity Pointer from iterator
					std::shared_ptr<Entity> player = *m_entityIt;
					// Recreate the ball object reference for Collision check
					std::shared_ptr<Entity> ball = *m_ballIt;
					CheckCollision(player, ball);

					int lives = player->GetPlayerLives();



					// TODO: DEBUG COLLIDE INDICATOR
					if (collided)
					{
						player->RemoveLife(m_time);

						lives = player->GetPlayerLives();
						m_soundController->playSound(hitSound, false);
						//// DEBUG RELEASE ENTITY
						//m_ballIt = m_ballVector.begin();
						////std::shared_ptr<Entity> ball = *m_ballIt;
						//m_ballIt = ReleaseBall(m_ballIt);
						collided = false;

						
						if (lives <=0){
							m_gameState = m_gameStates::LOST;
						}

						/*m_textController->Render(DirectX::SimpleMath::Vector2(m_windowWidth, m_windowHeight),
						xPercent, yPercent, intStr);*/
					}
				}

				// TODO: fix iterator deletion issue - miguel
				// Recreate the ball object reference for simulation
				std::shared_ptr<Entity> ball2 = *m_ballIt;

				// Put catched input into physx controller and update the entities position in the world
				m_physXController->StepPhysX(time, ball2->m_capturedInput->entityVector, ball2->GetDynamic());

				// Update the models
				m_modelController->Update(ball2);

				
			}
		}
}
#pragma endregion

#pragma region Frame Render
// Function : Draws the scene
void Game::Render()
{
	Clear();
	m_deviceResources->PIXBeginEvent(L"Render");
	auto context = m_deviceResources->GetD3DDeviceContext();
	
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	//const Value& jsonText = Parser::GetInstance().document["text"];

	if (m_gameState == m_gameStates::LOST)
	{

		// Render the text
		// TODO: RENDERING GAME MESSAGE
		m_modelController->RenderBackground(m_deviceResources.get(), m_camera.get());

		std::shared_ptr<Entity> player1 = m_entityVector[0];
		std::shared_ptr<Entity> player2 = m_entityVector[1];

		int p1Lives = player1->GetPlayerLives();
		int p2Lives = player2->GetPlayerLives();

		if (p1Lives > p2Lives)
		{
			player1->SetWinner("player1");
			winnerStatement = "   Player 1 Won!";
		}
		else if (p1Lives < p2Lives)
		{
			player2->SetWinner("player2");
			winnerStatement = "   Player 2 Won!";
		}
		else
		{
			player1->SetWinner("tie");
			winnerStatement = "   It's a tie!";
		}
		winnerStatement += "\nPress Y to Play Again";


		float xPercent = 0.85f;
		float yPercent = 0.5;
		//std::string title = jsonText["menu_text"]["title"]["text_body"].GetString();
		m_textController->Render(DirectX::SimpleMath::Vector2(m_windowWidth, m_windowHeight),
			xPercent, yPercent, winnerStatement, DirectX::Colors::White);
	}

	if (m_gameState == m_gameStates::MENU)
	{

		// Render the text
		// TODO: RENDERING DEBUG CODE
		m_modelController->RenderBackground(m_deviceResources.get(), m_camera.get());

		float xPercent = 0.75;
		float yPercent = 0.5;
		//std::string title = jsonText["menu_text"]["title"]["text_body"].GetString();
		m_textController->Render(DirectX::SimpleMath::Vector2(m_windowWidth, m_windowHeight),
			xPercent, yPercent, "Press A to start", DirectX::Colors::White);
	}

	if (m_gameState == m_gameStates::PLAYING)
	{

		// Don't try to render anything before the first Update.

		// playing
		// TODO: need to put a loop over a Vector to render physx according to many actors
		// Update the player 
		for (m_entityIt = m_entityVector.begin(); m_entityIt != m_entityVector.end(); ++m_entityIt)
		{
			// Init Entities Within the game
			std::shared_ptr<Entity> e = *m_entityIt;

			m_physXController->DrawActor(e->GetDynamic());
			// Render Models
			m_modelController->Render(m_deviceResources.get(), m_camera.get(), e);

		}

		// Renders dodge ball
		for (size_t i = 0; i < m_ballVector.size(); i++)
		{
			// Init Entities Within the game
			std::shared_ptr<Entity> e = m_ballVector[i];
			m_physXController->DrawActor(e->GetDynamic());

			// Render Models
			m_modelController->Render(m_deviceResources.get(), m_camera.get(), e);
		}

		// Update Camera resources
		m_inputController->UpdateCameraObject(m_camera);

		// Decrease time
		secondLoadoff = secondLoadoff + m_timer.GetElapsedSeconds();
		if (m_countdown >= 0.0);
		{
			// TODO: RENDERING DEBUG CODE
			if (secondLoadoff >= 1)
			{
				m_countdown -= 1;
			 	secondLoadoff = 0;
			}

		}

		// Rendering Text
		float xPercent = 0.55;
		float yPercent = 0.1;
		char timeChars[30];
		sprintf(timeChars, "%d", m_countdown);

		if (m_countdown <= 10){
			color1 = DirectX::Colors::Red;
		}
		else{
			color1 = DirectX::Colors::White;
		}
		m_textController->Render(DirectX::SimpleMath::Vector2(m_windowWidth, m_windowHeight),
			xPercent, yPercent, timeChars, color1);

		// TODO: Render Debug stats
		std::shared_ptr<Entity> player1 = m_entityVector[0];
		std::shared_ptr<Entity> player2 = m_entityVector[1];

		player1->UpdateInvincibilityStatus(m_time);
		player2->UpdateInvincibilityStatus(m_time);


		//Set life count for nboth players

		//Value for player1 lives
		char p1Life[30];
		sprintf(p1Life, "%d", player1->GetPlayerLives());
		//If invincibility is true set violet else set white
		if (player1->GetInvincibility() == true)
		{
			color1 = DirectX::Colors::BlueViolet;
		}
		else{
			color1 = DirectX::Colors::White;
		}
		//Sets p1's life count
		m_textController->Render(DirectX::SimpleMath::Vector2(m_windowWidth, m_windowHeight),
			0.3, 0.5, p1Life, color1);


		//Value for player1 lives
		char p2Life[30];
		sprintf(p2Life, "%d", player2->GetPlayerLives());
		//If invincibility is true set violet else set white
		if (player2->GetInvincibility() == true)
		{
			color2 = DirectX::Colors::BlueViolet;
		}
		else{
			color2 = DirectX::Colors::White;
		}
		//Sets p2's life count
		m_textController->Render(DirectX::SimpleMath::Vector2(m_windowWidth, m_windowHeight),
			0.7, 0.5, p2Life, color2);

	}

	// Set Device resources 
	m_deviceResources->PIXEndEvent();

	// Show the new frame.
	m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::Red);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
	if (m_gameState == m_gameStates::PLAYING)
	{
		m_inputController->ResumeActivateControls();
	}
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
	if (m_gameState == m_gameStates::PLAYING)
	{
		m_inputController->SuspendDeactivateControls();
	}
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
	if (m_gameState == m_gameStates::PLAYING)
	{
		m_inputController->SuspendDeactivateControls();
	}
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();
	if (m_gameState == m_gameStates::PLAYING)
	{
		m_inputController->ResumeActivateControls();
	}
    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

	if (m_gameState == m_gameStates::PLAYING)
	{
		// Update Width and Height Of Window
		m_windowHeight = height;
		m_windowWidth = width;
	}

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // nge to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();

		for (m_entityIt = m_entityVector.begin(); m_entityIt != m_entityVector.end(); ++m_entityIt)
		{
			// Init Entities Within the game
			std::shared_ptr<Entity> e = *m_entityIt;
			// Initialize device dependent objects here (independent of window size).
			// Init Model Controller
			m_modelController->InitDevices(m_deviceResources.get(), e);
		}

		// Creates balls inside ball vector
		for (m_ballIt = m_ballVector.begin(); m_ballIt != m_ballVector.end(); ++m_ballIt)
		{
			// Init Entities Within the game
			std::shared_ptr<Entity> e = *m_ballIt;
			// Initialize device dependent objects here (independent of window size).
			// Init Model Controller
			m_modelController->InitDevices(m_deviceResources.get(), e);
		}

		// Text Controller
		m_textController->InitDevices(m_deviceResources.get());
	
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{

	auto size = m_deviceResources->GetOutputSize();
	// TODO: Initialize windows-size dependent objects here.

	if (m_gameState == m_gameStates::MENU)
	{
		// Init Text Resources
		m_textController->InitResources((float)size.right, (float)size.bottom);
	}

	if (m_gameState == m_gameStates::PLAYING)
	{

		for (m_entityIt = m_entityVector.begin(); m_entityIt != m_entityVector.end(); ++m_entityIt)
		{
			// Init Entities Within the game
			std::shared_ptr<Entity> e = *m_entityIt;
			// Init Model Controller Window and Camera
			m_modelController->InitResources((float)size.right, (float)size.bottom, m_camera.get(), e);
		}

		// Creates Resource for Ball
		for (m_ballIt = m_ballVector.begin(); m_ballIt != m_ballVector.end(); ++m_ballIt)
		{
			// Init Entities Within the game
			std::shared_ptr<Entity> e = *m_ballIt;
			// Init Model Controller Window and Camera
			m_modelController->InitResources((float)size.right, (float)size.bottom, m_camera.get(), e);
		}

		// Init Text Resources
		m_textController->InitResources((float)size.right, (float)size.bottom);
	}
}
	


void
Game::SpawnBall(std::shared_ptr<Entity> player)
{
	// Load in a Json data for the Entity
	Parser::GetInstance().loadInFile("data.json");
	// Create new ball entity
	std::shared_ptr<Entity> ball = std::shared_ptr<Entity>(new Entity());
	
	// Check what type of data ball needs to be spawned
	if (player->m_name == "player1") 
	{
		const Value& jsonball = Parser::GetInstance().document["ball_test_entityA"];
		// Init ball entity
		ball->InitBall(jsonball, player);
	}
	if (player->m_name == "player2")
	{
		const Value& jsonball = Parser::GetInstance().document["ball_test_entityB"];
		// Init ball entity
		ball->InitBall(jsonball, player);
	}
	// Set Dynamic and add it to the gScene
	ball->SetDynamic(m_physXController->InitActor(ball));
	// Set Time of Birth by the time of the simulation
	ball->setTimeOfBirth(m_time);

	// plays the ball shot sound
	m_soundController->playSound(ballShot, false);

	// And add to the entity ball vector
	m_ballVector.push_back(ball);
}

std::vector<std::shared_ptr<Entity>>::iterator
Game::ReleaseEntity(std::vector<std::shared_ptr<Entity>>::iterator it)
{
	// Gets the second player
	std::shared_ptr<Entity> player = *it;

	// Removes from the scene
	m_physXController->DeleteActor(player->GetDynamic());
	player->release();

	// get position of iterator previous to erase
	auto position = it - m_entityVector.end() + 1;
	//	Erase from the vector
	m_entityVector.erase(m_entityIt);

	// Revalidate iterator if at the end of the vector 
	if (position == m_entityVector.size() - 1)
	{
		it = (m_entityVector.begin() + (m_entityVector.size() + 1));
	}
	else if (position == 0)
	{
		// If vector is now not empty
		if (m_entityVector.size() > 0)
			it = (m_entityVector.begin() + 1);
		// if vector is now empty
		else if (m_entityVector.size() == 0)
			it = (m_entityVector.end());

	}
	else // Revalidate iterator if not at the end of the vector
	{
		it = (m_entityVector.begin() + (position + 1));
	}

	return it;
}

std::vector<std::shared_ptr<Entity>>::iterator
Game::ReleaseBall(std::vector<std::shared_ptr<Entity>>::iterator it)
{
	// Gets the ball shared PTR
	std::shared_ptr<Entity> ball = *it;

	// Removes from the scene
	m_physXController->DeleteActor(ball->GetDynamic());
	ball->release();

	// get position of iterator previous to erase
	auto originalPosition = it - m_ballVector.end() + m_ballVector.size();
	//	Erase from the vector
	m_ballVector.erase(m_ballIt);

	// Revalidate iterator if at the end of the vector 
	if (originalPosition == m_ballVector.size() - 1)
	{
		it = (m_ballVector.begin() + (m_ballVector.size() - 1));
	}
	else if (originalPosition == 0)
	{
		// If vector is now not empty
		if (m_ballVector.size() > 0)
			it = (m_ballVector.begin()+1);
		// if vector is now empty
		else if (m_ballVector.size() == 0)
			it = (m_ballVector.begin());

	}
	else // Revalidate iterator if not at the end of the vector
	{
		it = (m_ballVector.begin() + (originalPosition + 1));
	}


	return it;
}


// Checks if the entity is hit
void Game::CheckCollision(std::shared_ptr<Entity> entity, std::shared_ptr<Entity> entity2)
{
		std::string playerName = entity->m_name;
		bool IsBallShot = entity->m_capturedInput.get()->m_ballShot;
		// This entity/Player
		float x1 = entity->GetDynamic()->getGlobalPose().p.x;
		float z1 = entity->GetDynamic()->getGlobalPose().p.z;
		float y1 = entity->GetDynamic()->getGlobalPose().p.y;

		/// ball
		float x2 = entity2->GetDynamic()->getGlobalPose().p.x;
		float z2 = entity2->GetDynamic()->getGlobalPose().p.z;
		float y2 = entity2->GetDynamic()->getGlobalPose().p.y;

		// Distance Collision Variable
		// TODO: May have to tune collision with the radius
		float collisionRadius = entity->actorData->dimensions.x * 4.45f;
		float actualdistance
			= sqrt(pow((x2 - x1), 2) + pow((z2 - z1), 2));
		// If hit set collision to true
		if (actualdistance < collisionRadius)
		{
			float actualdistanceY;
			actualdistanceY = y2 + y1;
			if (actualdistanceY == 0)
				actualdistanceY = 10;
			// check y pos
			if (actualdistanceY <
				collisionRadius)
				collided = true;
		}
}


void
Game::Release()
{
	m_modelController->Release();
}

// Function : Clean up Resources
void 
Game::OnDeviceLost()
{
    // Direct3D resource cleanup here.
	m_modelController.reset();
	m_textController.reset();
}

void 
Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
}
#pragma endregion
