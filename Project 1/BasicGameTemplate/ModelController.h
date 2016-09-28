#ifndef MODELCONTROLLER_H__
#define MODELCONTROLLER_H__

#include "pch.h"


// Forward Declarations:

class ModelController
{
	//Member Methods:
public:
	ModelController();
	~ModelController();

	int InitializeModelController();

	DirectX::SimpleMath::Matrix GetWorld();
	void SetMatrix(DirectX::SimpleMath::Matrix world);

	DirectX::SimpleMath::Matrix GetView();
	void SetView(DirectX::SimpleMath::Matrix view);

	DirectX::SimpleMath::Matrix GetProj();
	void SetProj(DirectX::SimpleMath::Matrix proj);

	std::unique_ptr<DirectX::CommonStates> GetStates();
	void SetStates(std::unique_ptr<DirectX::CommonStates> state);

	std::unique_ptr<DirectX::IEffectFactory> GetEffectFactory();
	void SetEffectFactory(std::unique_ptr<DirectX::CommonStates> factory);

	std::unique_ptr<DirectX::Model> GetModel();
	void SetModel(std::unique_ptr<DirectX::Model> model);
protected:

private:

	//Member Data:
public:

protected:

private:
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::IEffectFactory> m_fxFactory;
	std::unique_ptr<DirectX::Model> m_model;

};

#endif // MODELCONTROLLER_H__
