/**
DogeyBalls
ModelController.h
Purpose: Controls the model rendering 

@author Miguel Saavedra - Assisted by Sonic
@version 1.0 1/10/16
*/
#ifndef MODELCONTROLLER_H
#define MODELCONTROLLER_H
#include <SimpleMath.h>
#include <memory>
#include <CommonStates.h>
#include <Effects.h>
#include <Model.h>
#include "DeviceResources.h"


class Entity;

namespace physx {
	class PxRigidActor;
}

class ModelController
{
	
public: // Public Function Definitions
	ModelController();
	~ModelController();

	// Generalarized Template setter
	template <typename T>
	void SetVar(T&& value, T&& originalValue){ value = std::forward<T>(originalValue); }

	// Takes a pointer of the DeviceResources object
	// initialize all the device variables needed 
	// for model loading.
	// TODO: Add functioality to read in data from a json String to create Models
	void InitDevices(DX::DeviceResources* pdeviceRes);

	
	// the backBuffer width and height from the game class 
	// initialize all the resource variables needed 
	// for model loading.
	void InitResources(float backBufferWidth, float backBufferHeight);

	// Resets the pointers of the states, factory and model.
	void Reset();

	// Takes a pointer of the DeviceResources object,
	// then draws a Model
	// TODO: iterates through all models and draws them
	void Render(DX::DeviceResources* pdeviceRes);

	// Takes a time float then Updates the look of 
	// what model is being drawn
	void Update(float time, Entity* player);
public: // Public Member Definitions

	// Member objects
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	// Member pointers
	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::IEffectFactory> m_fxFactory;
	std::unique_ptr<DirectX::Model> m_model;

	std::unique_ptr<DirectX::BasicEffect> m_effect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	std::unique_ptr<DirectX::GeometricPrimitive> m_shape;

	float index = 0;

	// TODO: Add iterator and vector to hold unqiue model pointers

};

#endif