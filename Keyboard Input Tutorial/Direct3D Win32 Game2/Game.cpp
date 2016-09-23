//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

using namespace DirectX;

using Microsoft::WRL::ComPtr;

static const XMVECTORF32 START_POSITION = { 0.f, -1.5f, 0.f, 0.f };
static const XMVECTORF32 ROOM_BOUNDS = { 8.f, 6.f, 12.f, 0.f };
static const float ROTATION_GAIN = 0.004f;
static const float MOVEMENT_GAIN = 0.07f;

Game::Game() :
m_pitch(0),
m_yaw(0)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
	m_cameraPos = START_POSITION.v;
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

	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(window);
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
	auto kb = m_keyboard->GetState();
	if (kb.Escape)
		PostQuitMessage(0);

	auto mouse = m_mouse->GetState();
    elapsedTime;

	if (kb.Home)
	{
		m_cameraPos = START_POSITION.v;
		m_pitch = m_yaw = 0;
	}

	SimpleMath::Vector3 move = SimpleMath::Vector3::Zero;

	if (kb.Up || kb.W)
		move.y += 1.f;

	if (kb.Down || kb.S)
		move.y -= 1.f;

	if (kb.A)
		move.z += 1.f;

	if (kb.D)
		move.z -= 1.f;

	if (kb.Left )
		move.x += 1.f;

	if (kb.Right)
		move.x -= 1.f;

	if (kb.PageUp || kb.Space)
		move.z += 1.f;

	if (kb.PageDown || kb.X)
		move.z -= 1.f;

	SimpleMath::Quaternion q = SimpleMath::Quaternion::CreateFromYawPitchRoll(m_yaw, m_pitch, 0.f);

	move = SimpleMath::Vector3::Transform(move, q);

	move *= MOVEMENT_GAIN;

	m_cameraPos += move;

	SimpleMath::Vector3 halfBound = (SimpleMath::Vector3(ROOM_BOUNDS.v) / SimpleMath::Vector3(2.f))
		- SimpleMath::Vector3(0.1f, 0.1f, 0.1f);

	m_cameraPos = SimpleMath::Vector3::Min(m_cameraPos, halfBound);
	m_cameraPos = SimpleMath::Vector3::Max(m_cameraPos, -halfBound);
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
	float y = sinf(m_pitch);
	float r = cosf(m_pitch);
	float z = r*cosf(m_yaw);
	float x = r*sinf(m_yaw);

	XMVECTOR lookAt = m_cameraPos + SimpleMath::Vector3(x, y, z);

	XMMATRIX view = XMMatrixLookAtRH(m_cameraPos, lookAt, SimpleMath::Vector3::Up);

	m_room->Draw(SimpleMath::Matrix::Identity, view, m_proj, Colors::White, m_roomTex.Get());
	//float time = float(m_timer.GetTotalSeconds());

	//m_spriteBatch->Draw(m_texture.Get(), m_screenPos, nullptr, Colors::Red,
	//	cosf(time) * 4.f, m_origin,
	//	cosf(time) + 2.f);

	//m_spriteBatch->Begin();

	//m_spriteBatch->Draw(m_background.Get(), m_fullscreenRect);

	//m_spriteBatch->Draw(m_texture.Get(), m_screenPos, nullptr, Colors::White,
	//	0.f, m_origin);

	//m_spriteBatch->End();

	m_graphicsMemory->Commit();
    context;

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
	m_spriteBatch = std::make_unique<SpriteBatch>(m_deviceResources->GetD3DDeviceContext());

	ComPtr<ID3D11Resource> resource;
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(m_deviceResources->GetD3DDevice(), L"cat.dds",
		resource.GetAddressOf(),
		m_texture.ReleaseAndGetAddressOf()));

	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), L"sunset.jpg", nullptr,
		m_background.ReleaseAndGetAddressOf()));

	m_room = GeometricPrimitive::CreateBox(m_deviceResources->GetD3DDeviceContext(),
		XMFLOAT3(ROOM_BOUNDS[0], ROOM_BOUNDS[1], ROOM_BOUNDS[2]),
		false, true);

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(m_deviceResources->GetD3DDevice(), L"Sheep D.dds",
		nullptr, m_roomTex.ReleaseAndGetAddressOf()));

	ComPtr<ID3D11Texture2D> cat;
	DX::ThrowIfFailed(resource.As(&cat));

	CD3D11_TEXTURE2D_DESC catDesc;
	cat->GetDesc(&catDesc);

	m_origin.x = float(catDesc.Width / 2);
	m_origin.y = float(catDesc.Height / 2);

	m_fullscreenRect.left = 0;
	m_fullscreenRect.top = 0;
	auto size = m_deviceResources->GetOutputSize();
	m_fullscreenRect.right = m_screenPos.x = size.right;
	m_fullscreenRect.bottom = m_screenPos.y = size.bottom;

	m_graphicsMemory = std::make_unique<GraphicsMemory>(m_deviceResources->GetD3DDevice());
	m_states = std::make_unique<CommonStates>(m_deviceResources->GetD3DDevice());
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
	auto size = m_deviceResources->GetOutputSize();
	//m_screenPos.x = size.right / 2.f;
	//m_screenPos.y = size.bottom / 2.f;

	m_proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(XMConvertToRadians(70.f),
		float(size.right) / float(size.bottom), 0.01f, 100.f);
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
	m_texture.Reset();
	m_states.reset();
	m_background.Reset();
	m_room.reset();
	m_roomTex.Reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
