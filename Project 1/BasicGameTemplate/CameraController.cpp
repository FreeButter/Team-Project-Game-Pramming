/**
DogeyBalls
CameraController.cpp
Purpose: Controls the camera

@author vivian
@version 1.0 6/10/16
*/
#include "pch.h"
#include "CameraController.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

static const XMVECTORF32 START_POSITION = { 0.f, -1.5f, 0.f, 0.f };
static const XMVECTORF32 ROOM_BOUNDS = { 8.f, 6.f, 12.f, 0.f };
static const float ROTATION_GAIN = 0.004f;
static const float MOVEMENT_GAIN = 0.07f;

CameraController::CameraController() :
m_pitch(0),
m_yaw(0)
{
	m_cameraPos = START_POSITION.v;
}

CameraController::~CameraController()
{
}

void
CameraController::InitialiseCamera(HWND window)
{
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(window);
}

void CameraController::CameraUpdate(float elapsedTime)
{
	// TODO: Add your game logic here.
	auto kb = m_keyboard->GetState();
	if (kb.Escape)
		PostQuitMessage(0);

	auto mouse = m_mouse->GetState();

	if (kb.Home)
	{
		m_cameraPos = START_POSITION.v;
		m_pitch = m_yaw = 0;
	}

	SimpleMath::Vector3 move = SimpleMath::Vector3::Zero;

	if (kb.Up)
		move.y += 1.f;

	if (kb.Down)
		move.y -= 1.f;

	if (kb.W)
		move.z += 1.f;

	if (kb.S)
		move.z -= 1.f;

	if (kb.Left)
		move.x += 1.f;

	if (kb.Right)
		move.x -= 1.f;

	if (kb.A)
		m_yaw += tanf(elapsedTime)*1.0f;

	if (kb.D)
		m_yaw -= tanf(elapsedTime)*1.0f;

	if (kb.Q){
		if (m_pitch>-0.5)
			m_pitch -= tanf(elapsedTime)*1.0f;
	}

	if (kb.E){
		if (m_pitch<0.9)
			m_pitch += tanf(elapsedTime)*1.0f;
	}

	SimpleMath::Quaternion q = SimpleMath::Quaternion::CreateFromYawPitchRoll(m_yaw, m_pitch, 0.f);

	move = SimpleMath::Vector3::Transform(move, q);

	move *= MOVEMENT_GAIN;

	m_cameraPos += move;

	SimpleMath::Vector3 halfBound = (SimpleMath::Vector3(ROOM_BOUNDS.v) / SimpleMath::Vector3(2.f))
		- SimpleMath::Vector3(0.1f, 0.1f, 0.1f);

	m_cameraPos = SimpleMath::Vector3::Min(m_cameraPos, halfBound);
	m_cameraPos = SimpleMath::Vector3::Max(m_cameraPos, -halfBound);
}

void CameraController::RenderCamera(DX::DeviceResources* pdeviceRes){
	//m_model->Draw(m_deviceResources->GetD3DDeviceContext(), *m_states, m_world, m_view, m_proj);
	auto context = pdeviceRes->GetD3DDeviceContext();

	// TODO: Add your rendering code here.
	float y = sinf(m_pitch);
	float r = cosf(m_pitch);
	float z = r*cosf(m_yaw);
	float x = r*sinf(m_yaw);

	XMVECTOR lookAt = m_cameraPos + SimpleMath::Vector3(x, y, z);

	XMMATRIX view = XMMatrixLookAtRH(m_cameraPos, lookAt, SimpleMath::Vector3::Up);
	m_room->Draw(SimpleMath::Matrix::Identity, view, m_proj, Colors::White, m_roomTex.Get());

}

void CameraController::InitCameraDevices(DX::DeviceResources* pdeviceRes){
	// TODO: Initialize device dependent objects here (independent of window size).
	m_spriteBatch = std::make_unique<SpriteBatch>(pdeviceRes->GetD3DDeviceContext());

	ComPtr<ID3D11Resource> resource;
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(pdeviceRes->GetD3DDevice(), L"cat.dds",
		resource.GetAddressOf(),
		m_texture.ReleaseAndGetAddressOf()));

	DX::ThrowIfFailed(
		CreateWICTextureFromFile(pdeviceRes->GetD3DDevice(), L"sunset.jpg", nullptr,
		m_background.ReleaseAndGetAddressOf()));

	m_room = GeometricPrimitive::CreateBox(pdeviceRes->GetD3DDeviceContext(),
		XMFLOAT3(ROOM_BOUNDS[0], ROOM_BOUNDS[1], ROOM_BOUNDS[2]),
		false, true);

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(pdeviceRes->GetD3DDevice(), L"Sheep D.dds",
		nullptr, m_roomTex.ReleaseAndGetAddressOf()));

	ComPtr<ID3D11Texture2D> cat;
	DX::ThrowIfFailed(resource.As(&cat));

	CD3D11_TEXTURE2D_DESC catDesc;
	cat->GetDesc(&catDesc);

	m_origin.x = float(catDesc.Width / 2);
	m_origin.y = float(catDesc.Height / 2);

	m_fullscreenRect.left = 0;
	m_fullscreenRect.top = 0;
	auto size = pdeviceRes->GetOutputSize();
	m_fullscreenRect.right = m_screenPos.x = size.right;
	m_fullscreenRect.bottom = m_screenPos.y = size.bottom;

	m_graphicsMemory = std::make_unique<GraphicsMemory>(pdeviceRes->GetD3DDevice());
	m_states = std::make_unique<CommonStates>(pdeviceRes->GetD3DDevice());
}

void CameraController::InitCameraResources(float backBufferWidth, float backBufferHeight){
	m_proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(XMConvertToRadians(70.f),
		float(backBufferWidth) / float(backBufferHeight), 0.01f, 100.f);
}

void CameraController::Reset(){
	m_texture.Reset();
	m_states.reset();
	m_background.Reset();
	m_room.reset();
	m_roomTex.Reset();
}