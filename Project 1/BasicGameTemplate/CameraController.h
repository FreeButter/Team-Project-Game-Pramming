//
// CameraController.h
//

#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H
#include "pch.h"
#include "DeviceResources.h"
#include "StepTimer.h"


class CameraController
{
public:

	CameraController();
	~CameraController();
	// Initialization and management
	void InitialiseCamera(HWND window);
	void CameraUpdate(float elapsedTime);
	void RenderCamera(DX::DeviceResources* pdeviceRes);
	void InitCameraDevices(DX::DeviceResources* pdeviceRes);
	void InitCameraResources(float backBufferWidth, float backBufferHeight);
	void Reset();


public:

	std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	DirectX::SimpleMath::Vector2 m_screenPos;
	DirectX::SimpleMath::Vector2 m_origin;
	std::unique_ptr<DirectX::CommonStates> m_states;
	RECT m_tileRect;
	RECT m_fullscreenRect;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_background;

	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;

	std::unique_ptr<DirectX::GeometricPrimitive> m_room;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_roomTex;
	DirectX::SimpleMath::Matrix m_proj;
	DirectX::SimpleMath::Vector3 m_cameraPos;
	float m_pitch;
	float m_yaw;
};
#endif