#ifndef __INPUTMAPPER_H__
#define __INPUTMAPPER_H__

#include "pch.h"
#include "Parser.h"
#include <map>

class InputMapper
{
public: // functions
	InputMapper();
	~InputMapper();

	// Creates
	std::vector<DirectX::Keyboard::Keys> MapKeys(const Value& jsonControls);

public: // members
	map<std::string, DirectX::Keyboard::Keys> m_keyMap;


};

#endif //__INPUTMAPPER_H__

