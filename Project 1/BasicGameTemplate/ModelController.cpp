/**
DogeyBalls
ModelController.cpp
Purpose: Controls the model rendering

@author Miguel Saavedra
@version 1.0 1/10/16
*/
#include "pch.h"
#include "ModelController.h"
#include <PxRigidActor.h>
#include "Entity.h"

ModelController::ModelController()
{

}

ModelController::~ModelController()
{

}

namespace DX {
	class StepTimer;
}

void
ModelController::InitDevices(DX::DeviceResources* pdeviceRes, std::shared_ptr<Entity> player)
{
	//std::unique_ptr<DX::DeviceResources> UPdeviceRes;
	//UPdeviceRes = std::move(pdeviceRes);
	auto device = pdeviceRes->GetD3DDevice();
	// Device initialization

	//Grid Creation
	m_effect.reset(new DirectX::BasicEffect(device));
	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	DX::ThrowIfFailed(
		device->CreateInputLayout(DirectX::VertexPositionColor::InputElements,
		DirectX::VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.ReleaseAndGetAddressOf()));

	m_batch.reset(new DirectX::PrimitiveBatch<DirectX::VertexPositionColor>(pdeviceRes->GetD3DDeviceContext()));
	player->matrix = DirectX::SimpleMath::Matrix::Identity;

	// Entity
	m_shape = DirectX::GeometricPrimitive::CreateSphere(pdeviceRes->GetD3DDeviceContext(), 0.5f);
	m_square = DirectX::GeometricPrimitive::CreateCube(pdeviceRes->GetD3DDeviceContext(), 0.5f);

	// Entity Tex Init
	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(pdeviceRes->GetD3DDevice(), L"Assets/player1.jpg", nullptr,
		m_player1Tex.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(pdeviceRes->GetD3DDevice(), L"Assets/player2.jpg", nullptr,
		m_player2Tex.ReleaseAndGetAddressOf()));
	// Ground
	m_shapeGround = DirectX::GeometricPrimitive::CreateCube(pdeviceRes->GetD3DDeviceContext(), 20);
	// Sky Box
	m_skyBox = DirectX::GeometricPrimitive::CreateCube(pdeviceRes->GetD3DDeviceContext(), -70);
	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(pdeviceRes->GetD3DDevice(), L"Assets/dream.png", nullptr,
		m_texture.ReleaseAndGetAddressOf()));
	// Ground and Sky Matrix
	m_skyBoxMat = DirectX::SimpleMath::Matrix::Identity;
	m_groundMat = DirectX::SimpleMath::Matrix::Identity.CreateTranslation(DirectX::SimpleMath::Vector3(0, -10.0f, 0));

	// Creating background for start screen
	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(pdeviceRes->GetD3DDevice(), L"Assets/cat.jpg", nullptr,
		m_background.ReleaseAndGetAddressOf()));

	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(pdeviceRes->GetD3DDeviceContext());

}

void
ModelController::InitResources(float backBufferWidth, float backBufferHeight, Camera* camera,
	std::shared_ptr<Entity> player)
{
	// Resource initialization

	// Create Projection and view from Camera
	m_proj = camera->getProjection();
	m_view = camera->getView();

	// Set the world, view and projection
	player->matrix = DirectX::SimpleMath::Matrix::Identity;
	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);
}

void
ModelController::Reset()
{
	// Reset the Pointers
	m_states.reset();
	m_fxFactory.reset();
	m_model.reset();
	m_background.Reset();
}

void
ModelController::Render(DX::DeviceResources* pdeviceRes, Camera* camera, std::shared_ptr<Entity> player)
{
	//Draws Sword
	ID3D11DeviceContext* deviceContext = pdeviceRes->GetD3DDeviceContext();

	//Draws Box
	if (player->entityType == Entity::type::box && player->m_name == "player1")
		m_square->Draw(player->matrix, camera->getView(), camera->getProjection(), DirectX::Colors::Red);
	if (player->entityType == Entity::type::box && player->m_name == "player2")
		m_square->Draw(player->matrix, camera->getView(), camera->getProjection(), DirectX::Colors::Blue);
	if (player->entityType == Entity::type::ball)
		m_shape->Draw(player->matrix, camera->getView(), camera->getProjection());
	//Draws Ground
	m_shapeGround->Draw(m_groundMat, camera->getView(), camera->getProjection(), DirectX::Colors::Black);
	//Draws skyBox
	m_skyBox->Draw(m_skyBoxMat, camera->getView(), camera->getProjection(), DirectX::Colors::White, m_texture.Get());

	// Set Matrix
	auto context = pdeviceRes->GetD3DDeviceContext();
	m_effect->SetWorld(player->matrix);
	m_effect->Apply(pdeviceRes->GetD3DDeviceContext());
	pdeviceRes->GetD3DDeviceContext()->IASetInputLayout(m_inputLayout.Get());

}

void 
ModelController::RenderBackground(DX::DeviceResources* pdeviceRes, Camera* camera)
{
	m_spriteBatch->Begin();

	m_spriteBatch->Draw(m_background.Get(), m_screenPos, nullptr, DirectX::Colors::White, 0.f, m_origin);

	m_spriteBatch->End();
}

void
ModelController::Update(std::shared_ptr<Entity> player)
{
	//Rotates the objects in the world
	DirectX::SimpleMath::Matrix m = DirectX::SimpleMath::Matrix();
	physx::PxMat33 pm_mat33m = physx::PxMat33(player->dynamic->getGlobalPose().q);

	//m_world = m.CreateRotationY((time) *	1.f);

	float x = player->dynamic->getGlobalPose().p[0];
	float y = player->dynamic->getGlobalPose().p[1];
	float z = player->dynamic->getGlobalPose().p[2];

	float qx0 = pm_mat33m.column0[0];
	float qy0 = pm_mat33m.column0[1];
	float qz0 = pm_mat33m.column0[2];

	float qx1 = pm_mat33m.column1[0];
	float qy1 = pm_mat33m.column1[1];
	float qz1 = pm_mat33m.column1[2];

	float qx2 = pm_mat33m.column2[0];
	float qy2 = pm_mat33m.column2[1];
	float qz2 = pm_mat33m.column2[2];

	//DirectX::SimpleMath::Vector3 pos(x,y,z);

	player->matrix = DirectX::SimpleMath::Matrix(qx0, qy0, qz0
		, 0, qx1, qy1, qz1
		, 0, qx2, qy2, qz2
		, 0, x, y, z, 1);

	//DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3::Lerp(DirectX::SimpleMath::Vector3(0, 1, 0), 
	//	DirectX::SimpleMath::Vector3(0, 0, 0), time);


	//m_world = m.CreateTranslation(pos); // put in here
	//m_world = m.CreateWorld(pos, DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3::Up);

	// * m.CreateRotationY(time);
}

