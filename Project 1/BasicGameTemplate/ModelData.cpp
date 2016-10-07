/**
DogeyBalls
ModelData.cpp
Purpose: Stores the data needed to manipulate the movement,
scale and rotation of the model object

@author Miguel Saavedra
@version 1.0 6/10/16
*/
#include "pch.h"
#include "ModelData.h"

ModelData::ModelData(): 
fileLoc(0),
name(0),
degrees(0)
{
	
}

ModelData::~ModelData()
{
	
}

void 
ModelData::SetModel(const char fileLoc)
{
	// TODO: init model
}

void 
ModelData::SetNewVector(float x, float y, float z)
{
	*m_pos = DirectX::SimpleMath::Vector3(z, y, z);
}

void 
ModelData::SetNewRotation(float rotation)
{
	degrees = rotation;
}

// TODO: Set appropriate value as parameter
void
ModelData::SetNewTransform()
{
	
}
