#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Logger.h"
#include "Screen.h"

class ErrorScreen : public Screen
{
public:
	ErrorScreen(CharacterCore* core);

	void render(sf::RenderTarget& renderTarget) override;
	Screen* update(const sf::Time& frameTime) override;

	void execOnEnter(const Screen* previousScreen) override;
	void execOnExit(const Screen* nextScreen) override;

private:
	sf::Sprite m_screenSprite;
	sf::Text m_errorText;
	ResourceID m_screenResource;
};