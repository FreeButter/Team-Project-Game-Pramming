/**
DogeyBalls
ModelController.h
Purpose: Stores the data needed to manipulate the movement, 
scale and rotation of the model object

Members are Public as Entity is the only accessor of the object

@author Miguel Saavedra
@version 1.0 6/10/16
*/
#ifndef __MODELDATA_H__
#define __MODELDATA_H__
#include <memory>
#include <Model.h>

// Forward Declarations:

class ModelData
{
	//Member Methods:
public:
	ModelData();
	~ModelData();

	// Setters 

	//// Sets up the model object
	//void SetModel(const char fileLoc);

	//// Sets the new Position of the model
	//// Takes x y z float values to manipulate
	//// the next place the model is drawn
	//void SetNewVector(float x, float y, float z);

	//// Sets the rotation of the model 
	//// Takes a float value representing degrees
	//void SetNewRotation(float rotation);

	//// TODO: Set appropriate value as parameter
	//void SetNewTransform();

protected:

private:
	//Member Data:
public:
	// Unique Pointer to the model object
	std::unique_ptr<DirectX::Model> m_model;

	//// Name of the Model
	//std::string name;

	// Location of Model
	const char fileLoc;

	//// Unique pointer to Vertex Position of Model
	//std::unique_ptr<DirectX::SimpleMath::Vector3> m_pos;

	//// Rotation Variable
	//float degrees;

	//// Scale/Transform Variable
	//// TODO: Set appropriate variable here to control Scaling


	

protected:

private:

};

#endif //__MODELDATA_H__
