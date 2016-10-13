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
ModelController::InitDevices(DX::DeviceResources* pdeviceRes)
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
	m_world = DirectX::SimpleMath::Matrix::Identity;

	////SWORD
	//m_states = std::make_unique<DirectX::CommonStates>(device);

	////m_fxFactory = std::make_unique<DirectX::EffectFactory>(device);
	//m_fxFactory = std::make_unique<DirectX::DGSLEffectFactory>(device);

	//m_model = DirectX::Model::CreateFromCMO(device, L"sword.cmo", *m_fxFactory);


	// Initialize lights and fog
	//m_model->UpdateEffects([](DirectX::IEffect* effect)
	/*{
		auto lights = dynamic_cast<DirectX::IEffectLights*>(effect);
		if (lights)
		{
			lights->SetLightingEnabled(true);
			lights->SetPerPixelLighting(true);
			lights->SetLightEnabled(0, true);
			lights->SetLightDiffuseColor(0, DirectX::Colors::Gold);
			lights->SetLightEnabled(1, false);
			lights->SetLightEnabled(2, false);
		}

		auto fog = dynamic_cast<DirectX::IEffectFog*>(effect);
		if (fog)
		{
			fog->SetFogEnabled(true);
			fog->SetFogColor(DirectX::Colors::CornflowerBlue);
			fog->SetFogStart(3.f);
			fog->SetFogEnd(4.f);
		}
	});*/

	//Creates Box, Size x 2

		m_shape = DirectX::GeometricPrimitive::CreateSphere(pdeviceRes->GetD3DDeviceContext(), 0.5f);

}

void
ModelController::InitResources(float backBufferWidth, float backBufferHeight)
{
	// Resource initialization
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(0.f, 4.f, 6.f),
		DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 2.5f,
		float(backBufferWidth) / float(backBufferHeight), 0.1f, 100.f);

	DirectX::SimpleMath::Matrix proj = DirectX::SimpleMath::Matrix::CreateScale(2.f / float(backBufferWidth),
		-2.f / float(backBufferHeight), 1.f)
		* DirectX::SimpleMath::Matrix::CreateTranslation(-1.f, 1.f, 0.f);
	m_effect->SetProjection(proj);
	m_world = DirectX::SimpleMath::Matrix::Identity;
	//m_view = DirectX::SimpleMath::Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(2.f, 2.f, 2.f), DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);
	//m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.f,
	//	float(backBufferWidth) / float(backBufferHeight), 0.1f, 10.f);

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
}

void 
ModelController::Render(DX::DeviceResources* pdeviceRes)
{ 
	//Draws Sword
	ID3D11DeviceContext* deviceContext = pdeviceRes->GetD3DDeviceContext();
	
	//m_model->Draw(deviceContext, *m_states, m_world, m_view, m_proj);

	//Draws Box
	m_shape->Draw(m_world, m_view, m_proj);

	auto context = pdeviceRes->GetD3DDeviceContext();
	
	m_effect->SetWorld(m_world);

	m_effect->Apply(pdeviceRes->GetD3DDeviceContext());

	pdeviceRes->GetD3DDeviceContext()->IASetInputLayout(m_inputLayout.Get());

	m_batch->Begin();

	DirectX::SimpleMath::Vector3 xaxis(3.f, 0.f, 0.f);
	DirectX::SimpleMath::Vector3 yaxis(0.f, 0.f, 3.f);
	DirectX::SimpleMath::Vector3 origin(0, 0, 0);

	//index+=0.001;

		//= DirectX::SimpleMath::Vector3::Zero;
	m_world = DirectX::SimpleMath::Matrix::CreateTranslation(origin);
	m_effect->SetWorld(m_world);
	m_effect->Apply(pdeviceRes->GetD3DDeviceContext());


	// Draw Grid
	size_t divisions = 20;

	for (size_t i = 0; i <= divisions; ++i)
	{
		float fPercent = float(i) / float(divisions);
		fPercent = (fPercent * 2.0f) - 1.0f;

		DirectX::SimpleMath::Vector3 scale = xaxis * fPercent + origin;

		DirectX::VertexPositionColor v1(scale - yaxis, DirectX::Colors::White);
		DirectX::VertexPositionColor v2(scale + yaxis, DirectX::Colors::White);
		m_batch->DrawLine(v1, v2);
	}

	for (size_t i = 0; i <= divisions; i++)
	{
		float fPercent = float(i) / float(divisions);
		fPercent = (fPercent * 2.0f) - 1.0f;

		DirectX::SimpleMath::Vector3 scale = yaxis * fPercent + origin;

		DirectX::VertexPositionColor v1(scale - xaxis, DirectX::Colors::White);
		DirectX::VertexPositionColor v2(scale + xaxis, DirectX::Colors::White);
		m_batch->DrawLine(v1, v2);
	}

	m_batch->End();



}

void
ModelController::Update(float time, Entity *player)
{
	//Rotates the objects in the world
	DirectX::SimpleMath::Matrix m = DirectX::SimpleMath::Matrix();
	physx::PxMat33 pm_mat33m = physx::PxMat33(player->actor->getGlobalPose().q);

	//m_world = m.CreateRotationY((time) *	1.f);

	float x = player->actor->getGlobalPose().p[0];
	float y = player->actor->getGlobalPose().p[1];
	float z = player->actor->getGlobalPose().p[2];
	
	y -= 2 * time;
	
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

	m_world = DirectX::SimpleMath::Matrix(qx0, qy0, qz0
										, 0, qx1, qy1, qz1
										, 0, qx2, qy2, qz2
										, 0, x, y, z, 1);

	//DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3::Lerp(DirectX::SimpleMath::Vector3(0, 1, 0), 
	//	DirectX::SimpleMath::Vector3(0, 0, 0), time);
	
	
	//m_world = m.CreateTranslation(pos); // put in here
	//m_world = m.CreateWorld(pos, DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3::Up);

		// * m.CreateRotationY(time);
}

