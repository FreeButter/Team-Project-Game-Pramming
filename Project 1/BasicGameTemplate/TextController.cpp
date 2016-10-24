/**
DogeyBalls
TextController.cpp
Purpose: Controls the Text

@author Sonic
@version 1.0 1/10/16
*/
#include "pch.h"
#include <memory>
#include "TextController.h"
#include "StepTimer.h"
#include "DeviceResources.h"

TextController::TextController()
{

}

TextController::~TextController()
{

}

// Will be used for FPS Counter
void
TextController::InitConsole()
{
	//m_console = std::make_unique<TextConsole>();

	// Optionally set a color other than white with something like:
	// m_console->SetForegroundColor(Colors::Yellow);
}

// Initilizes the Sprites and the Font
void
TextController::InitDevices(DX::DeviceResources* pdeviceRes)
{
	//m_console->RestoreDevice(pdeviceRes->GetD3DDeviceContext(), L"consolas.ttf");
	m_font = std::make_unique<DirectX::SpriteFont>(pdeviceRes->GetD3DDevice(), L"myfile.spritefont");
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(pdeviceRes->GetD3DDeviceContext());
}

// Position of Sprite on screen
void
TextController::InitResources(float backBufferWidth, float backBufferHeight)
{
	//m_console->SetWindow(DirectX::SimpleMath::Viewport::ComputeTitleSafeArea(backBufferWidth, backBufferHeight));
	m_fontPos.x = backBufferWidth / 2;
	m_fontPos.y = backBufferHeight / 2;
}

// Resets the Font and sprite batch
void
TextController::Reset()
{
	//m_console->ReleaseDevice();
	m_font.reset();
	m_spriteBatch.reset();
}

// Renders the Colour and Text
void
TextController::Render(DirectX::SimpleMath::Vector2 textPos, 
float xPercent, float yPercent, std::string renderString, DirectX::FXMVECTOR color)
{
	// Uncomment to render console on screen
	//m_console->Render();
	
	// Initialize Sprite rendering
	m_spriteBatch->Begin();

	// Convert String into wString
	std::wstring name = std::wstring(renderString.begin(), renderString.end());
	
	// Set Characters being used from wString
	const wchar_t* output =	name.c_str();

	// Set Position by positioning text according to the screen dimensions
	textPos.x = textPos.x * xPercent;
	textPos.y = textPos.y * yPercent;

	DirectX::SimpleMath::Vector2 origin = m_font->MeasureString(output);

	m_font->DrawString(m_spriteBatch.get(), output,
		textPos, color, 0.f, origin);

	// Finish drawing
	m_spriteBatch->End();
}

// For FPS Counter
void
TextController::WriteLine()
{
	//m_console->WriteLine(L"This is a test");
	//m_console->WriteLine(L"Line 2");
}