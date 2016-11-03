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

	// Shooting
	m_keyMap["f"] = Keyboard::Keys::F;

	// Jumping
	m_keyMap["space"] = Keyboard::Keys::Space;

	// Player 2 Movement
	m_keyMap["up"] = Keyboard::Keys::Up;
	m_keyMap["left"] = Keyboard::Keys::Left;
	m_keyMap["down"] = Keyboard::Keys::Down;
	m_keyMap["right"] = Keyboard::Keys::Right;

	// Shooting
	m_keyMap["numpad_2"] = Keyboard::Keys::NumPad2;

	// Jumping
	m_keyMap["numpad_1"] = Keyboard::Keys::NumPad1;
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

	// Pushing Back Directions in 0,1,2,3 WASD order // 4(f & numpad 2) is Shoot, 5(space & Numpad 1) is Jump
	for (int i = 0; i < 6; i++)
	{
		// take values from jsonControls variable
  		string keyid = jsonControls["control_scheme"]["DIRECTIONS"][i].GetString();
		controls.push_back(m_keyMap[keyid]);
	}

	// return vector
	return controls;
}