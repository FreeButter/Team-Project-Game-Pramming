/**
DogeyBalls
TextController.h
Purpose: Controls the Text

@author Sonic
@version 1.0 1/10/16
*/
#ifndef TEXTCONTROLLER_H
#define TEXTCONTROLLER_H

#include "pch.h"
#include <memory>
#include "DeviceResources.h"
#include "TextConsole.h"


namespace DX {
	class StepTimer;
}

class Entity;

class TextController
{

public: // Public Function Definitions
	TextController();
	~TextController();

	// Currently not in use, For FPS counter
	void InitConsole();

	// Initializes the Text
	void InitDevices(DX::DeviceResources* pdeviceRes);

	// Initializes the Resources needed by Text
	void InitResources(float backBufferWidth, float backBufferHeight);

	// Resets the Text
	void Reset();

	// Renders the Text
	// Takes a 2D vector for the screen size
	// and two floats for positioning the text from
	// the LHS e.g. x = 0.2 therefore the text will be rendered
	// 20 percent of the width of the window from the LHS,
	// it also takes a string which will be rendered on the screen
	void Render(DirectX::SimpleMath::Vector2 textPos, 
		float xPercent, float yPercent, std::string renderString, DirectX::FXMVECTOR color);

	// Not used, Will be used for FPS Counter
	void WriteLine();

public:

	//std::unique_ptr<TextConsole> m_console;

	// Font Sprite Variable
	std::unique_ptr<DirectX::SpriteFont> m_font;
	DirectX::SimpleMath::Vector2 m_fontPos;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
};

#endif