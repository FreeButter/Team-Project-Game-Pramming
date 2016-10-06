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
#include "CameraController.h"


// A basic game implementation that creates a D3D11 device and
// provides a game loop. 
class Game : public DX::IDeviceNotify
{
public:

    Game();

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

private:

    void Update(DX::StepTimer const& timer);

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;
	//DirectX::SimpleMath::Matrix m_world;
	//DirectX::SimpleMath::Matrix m_view;
	//DirectX::SimpleMath::Matrix m_proj;

	//std::unique_ptr<DirectX::CommonStates> m_states;
	//std::unique_ptr<DirectX::IEffectFactory> m_fxFactory;
	//std::unique_ptr<DirectX::Model> m_model;

	// Controller Unique Pointers

	std::unique_ptr<ModelController> m_modelController;
	std::unique_ptr<CameraController> m_cameraController;
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

	//
	
};

#endif