//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game()
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
	// TODO: Create Controllers unique pointers
	m_modelController = std::unique_ptr<ModelController>(new ModelController());
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

	
	
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

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
	
	float time = float(timer.GetTotalSeconds());
	m_modelController->Update(time);
	//m_world = SimpleMath::Matrix::CreateRotationZ(cosf(time) * 2.f);
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.
	m_modelController->Draw(m_deviceResources.get());
	//m_model->Draw(m_deviceResources->GetD3DDeviceContext(), *m_states, m_world, m_view, m_proj);
    

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

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
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
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();
    
	// TODO: Initialize device dependent objects here (independent of window size).
	m_modelController->InitDevices(m_deviceResources.get());

	/*m_states = std::make_unique<CommonStates> (m_deviceResources->GetD3DDevice());

	m_fxFactory = std::make_unique<DGSLEffectFactory>(m_deviceResources->GetD3DDevice());

	m_model = Model::CreateFromCMO(m_deviceResources->GetD3DDevice(), L"cup.cmo", *m_fxFactory);

	m_world = SimpleMath::Matrix::Identity;

	m_model->UpdateEffects([](IEffect* effect)
	{
		auto lights = dynamic_cast<IEffectLights*>(effect);
		if (lights)
		{
			lights->SetLightingEnabled(true);
			lights->SetPerPixelLighting(true);
			lights->SetLightEnabled(0, true);
			lights->SetLightDiffuseColor(0, Colors::Gold);
			lights->SetLightEnabled(1, false);
			lights->SetLightEnabled(2, false);
		}

		auto fog = dynamic_cast<IEffectFog*>(effect);
		if (fog)
		{
			fog->SetFogEnabled(true);
			fog->SetFogColor(Colors::CornflowerBlue);
			fog->SetFogStart(3.f);
			fog->SetFogEnd(4.f);
		}
	});*/
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
	auto size = m_deviceResources->GetOutputSize();

	m_modelController->InitResources((float)size.right, (float)size.bottom);
	/*m_view = SimpleMath::Matrix::CreateLookAt(SimpleMath::Vector3(2.f, 2.f, 2.f),
	                                          SimpleMath::Vector3::Zero, SimpleMath::Vector3::UnitY);
	m_proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(size.right) / float(size.bottom), 0.1f, 10.f);*/
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
	m_modelController.reset();
	//m_states.reset();
	//m_fxFactory.reset();
	//m_model.reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
