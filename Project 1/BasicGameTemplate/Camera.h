/**
DogeyBalls
Camera.h
Purpose: Store data about the cameras
behaivour and state

@author Miguel Saavedra
@version 1.0 6/10/16
*/
#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "pch.h"
#include "DeviceResources.h"

class Camera
{
	//Member Methods:
public:
	Camera();
	~Camera();
	// Initialize Camera Properties
	void InitCamera(float backBufferWidth, float backBufferHeight, 
		DirectX::SimpleMath::Vector3 cameraPosition);

	// Setters
	void SetPosition(DirectX::SimpleMath::Vector3 position);
	void SetView(DirectX::SimpleMath::Matrix view);
	void SetPitch(float pitch);
	void SetYaw(float yaw);

	// Getters
	float GetPitch();
	float GetYaw();
	DirectX::SimpleMath::Vector3 getPosition();
	DirectX::SimpleMath::Matrix getView();
	DirectX::SimpleMath::Matrix getProjection();



protected:

private:

	// Member Data
public:

	// Lookat
	DirectX::XMVECTOR lookAt;

	std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;
	// Projection
	DirectX::SimpleMath::Matrix m_proj;
	// View
	DirectX::SimpleMath::Matrix m_view;
	// Position
	DirectX::SimpleMath::Vector3 m_cameraPos;

	// Angular Positioning
	float m_pitch;
	float m_yaw;

	// Camera Acceleration
	float ROTATION_GAIN;
	float MOVEMENT_GAIN;




protected:

private:


};

#endif //__CAMERA_H__