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
#include "Camera.h"


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
	// TODO: Add functionality to read in data from a json String to create Models
	void InitDevices(DX::DeviceResources* pdeviceRes, std::shared_ptr<Entity> player);


	// the backBuffer width and height from the game class 
	// initialize all the resource variables needed 
	// for model loading.
	void InitResources(float backBufferWidth, float backBufferHeight, Camera* camera,
		std::shared_ptr<Entity> player);

	// Resets the pointers of the states, factory and model.
	void Reset();

	// Takes a pointer of the DeviceResources object,
	// then draws a Model
	// TODO: iterates through all models and draws them
	void Render(DX::DeviceResources* pdeviceRes, Camera* camera, std::shared_ptr<Entity> player);
	void RenderBackground(DX::DeviceResources* pdeviceRes, Camera* camera);

	// Takes a time float then Updates the look of 
	// what model is being drawn
	void Update(std::shared_ptr<Entity> player);
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

	// Shap Pointers
	std::unique_ptr<DirectX::GeometricPrimitive> m_room;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_roomTex;
	// Debug Entity
	std::unique_ptr<DirectX::GeometricPrimitive> m_shape;
	std::unique_ptr<DirectX::GeometricPrimitive> m_square;
	// Ground
	std::unique_ptr<DirectX::GeometricPrimitive> m_shapeGround;
	DirectX::SimpleMath::Matrix m_groundMat;
	// Sky
	std::unique_ptr<DirectX::GeometricPrimitive> m_skyBox;
	DirectX::SimpleMath::Matrix m_skyBoxMat;
	// Stating Screen Background
	RECT m_fullscreenRect;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_background;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	DirectX::SimpleMath::Vector2 m_screenPos;
	DirectX::SimpleMath::Vector2 m_origin;


	float index = 0;

	// Textures
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// Entity Textures
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_player1Tex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_player2Tex;

};

#endif