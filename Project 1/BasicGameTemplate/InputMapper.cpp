#include "pch.h"
#include "InputMapper.h"

using namespace DirectX;

InputMapper::InputMapper()
{
	// init keymap
	// Player 1 Movement
	m_keyMap = map<std::string, Keyboard::Keys>();
	m_keyMap["w"] = Keyboard::Keys::W;
	m_keyMap["a"] = Keyboard::Keys::A;
	m_keyMap["s"] = Keyboard::Keys::S;
	m_keyMap["d"] = Keyboard::Keys::D;

	// Player 2 Movement
	m_keyMap["up"] = Keyboard::Keys::Up;
	m_keyMap["left"] = Keyboard::Keys::Left;
	m_keyMap["down"] = Keyboard::Keys::Down;
	m_keyMap["right"] = Keyboard::Keys::Right;
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

	for (int i = 0; i < 4; i++)
	{
		// take values from jsonControls variable
  		string keyid = jsonControls["control_scheme"]["DIRECTIONS"][i].GetString();
		controls.push_back(m_keyMap[keyid]);
	}

	// put into vector


	// return vector
	return controls;
}