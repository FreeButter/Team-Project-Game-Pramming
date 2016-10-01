/**
DogeyBalls
ModelController.cpp
Purpose: Controls the model rendering

@author Miguel Saavedra
@version 1.0 1/10/16
*/
#include "pch.h"
#include "ModelController.h"

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
	m_states = std::make_unique<DirectX::CommonStates>(device);

	//m_fxFactory = std::make_unique<DirectX::EffectFactory>(device);
	m_fxFactory = std::make_unique<DirectX::DGSLEffectFactory>(device);

	m_model = DirectX::Model::CreateFromCMO(device, L"cup.cmo", *m_fxFactory);

	// Initialize lights and fog
	m_model->UpdateEffects([](DirectX::IEffect* effect)
	{
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
	});
}

void
ModelController::InitResources(float backBufferWidth, float backBufferHeight)
{
	// Resource initialization
	m_world = DirectX::SimpleMath::Matrix::Identity;

	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(2.f, 2.f, 2.f),
		DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.f,
		float(backBufferWidth) / float(backBufferHeight), 0.1f, 10.f);

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
ModelController::Draw(DX::DeviceResources* pdeviceRes)
{ 
	ID3D11DeviceContext* deviceContext = pdeviceRes->GetD3DDeviceContext();
	m_model->Draw(deviceContext, *m_states, m_world, m_view, m_proj);
}

void
ModelController::Update(float time)
{
	m_world = DirectX::SimpleMath::Matrix::CreateRotationZ(cosf(time) * 2.f);
}

