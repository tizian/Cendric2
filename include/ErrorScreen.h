#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Logger.h"
#include "Screen.h"

class ErrorScreen : public Screen
{
public:
	ErrorScreen();

	void render(sf::RenderTarget &renderTarget) override;

	void onEnter(Screen *previousScreen) override;
	void onExit(Screen *nextScreen) override;

private:
	sf::Sprite m_screenSprite;
	sf::Text m_errorText;
	ResourceID m_screenResource;
};