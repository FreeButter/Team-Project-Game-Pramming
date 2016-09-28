#include "pch.h"
#include "ModelController.h"

using namespace DirectX::SimpleMath;
using namespace std;

ModelController::ModelController()
	:
	m_world(0),
	m_view(0),
	m_proj(0),
	m_states(make_unique<DirectX::CommonStates>(0)),
	m_fxFactory(make_unique<DirectX::IEffectFactory>()),
	m_model(make_unique<DirectX::Model>())
{
	
}

