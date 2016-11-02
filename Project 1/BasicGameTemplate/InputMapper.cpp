#include "pch.h"
#include "InputMapper.h"

using namespace DirectX;

InputMapper::InputMapper()
{
	// init keymap
	m_keyMap = map<std::string, Keyboard::Keys>();
	m_keyMap["w"] = Keyboard::Keys::W;
}
InputMapper::~InputMapper()
{
	
}


// Creates
std::vector<DirectX::Keyboard::Keys> 
InputMapper::MapKeys(const Value& jsonControls)
{
	// Init control vector
	std::vector<DirectX::Keyboard::Keys> controls = std::vector<DirectX::Keyboard::Keys>();

	// take values from jsonControls variable
	string keyid = jsonControls["control_scheme"]["UP"].GetString();
	controls.push_back(m_keyMap[keyid]);


	// put into vector


	// return vector
	return controls;
}