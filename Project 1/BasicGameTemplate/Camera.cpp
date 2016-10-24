#include "pch.h"
#include "camera.h"

// Camera position
static const DirectX::XMVECTORF32 START_POSITION = { 0.f, -1.5f, 0.f, 0.f };

Camera::Camera() {

}

Camera::~Camera() {

}

void
Camera::InitCamera(float backBufferWidth, float backBufferHeight, DirectX::SimpleMath::Vector3 cameraPos)
{

	//float y = sinf(m_pitch);
	//float r = cosf(m_pitch);
	//float z = r*cosf(m_yaw);
	//float x = r*sinf(m_yaw);

	//DirectX::XMVECTOR lookAt = cameraPos + DirectX::SimpleMath::Vector3(x, y, z);
	//m_view = XMMatrixLookAtRH(cameraPos, lookAt, DirectX::SimpleMath::Vector3::Up);

	// View
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(0.f, 4.f, 6.f),
		DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Up);

	// Projection
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 2.5f,
		float(backBufferWidth) / float(backBufferHeight), 0.1f, 100.f);

	// Camera Position
	m_cameraPos = cameraPos;

	// Angular Positioning
	m_pitch = -0.5;
	m_yaw = 0;
	// Acceleration
	ROTATION_GAIN = 1;
	MOVEMENT_GAIN = 0.1;
}

DirectX::SimpleMath::Matrix
Camera::getView(){
	//return DirectX::SimpleMath::Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(0.f, 4.f, 6.f),
	//	DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);
	return m_view;
}

DirectX::SimpleMath::Matrix
Camera::getProjection()
{
	//return DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 2.5f,
	//	float(backBufferWidth) / float(backBufferHeight), 0.1f, 100.f);
	return m_proj;
}

void 
Camera::SetPosition(DirectX::SimpleMath::Vector3 position)
{
	m_cameraPos = (position);
}

DirectX::SimpleMath::Vector3
Camera::getPosition()
{
	return m_cameraPos;
}

void 
Camera::SetView(DirectX::SimpleMath::Matrix view)
{
	m_view = (view);
}

float 
Camera::GetPitch()
{
	return m_pitch;
}

float 
Camera::GetYaw()
{
	return m_yaw;
}

void 
Camera::SetPitch(float pitch)
{
	m_pitch = pitch;
}

void 
Camera::SetYaw(float yaw)
{
	m_yaw = yaw;
}
