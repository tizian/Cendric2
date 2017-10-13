#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Logger.h"
#include "Screen.h"

class ErrorScreen final : public Screen {
public:
	ErrorScreen(CharacterCore* core);

	void render(sf::RenderTarget& renderTarget) override;
	void execUpdate(const sf::Time& frameTime) override;

	void execOnEnter(Screen* previousScreen) override;
	void execOnExit(Screen* nextScreen) override;

private:
	sf::Sprite m_screenSprite;
	BitmapText m_errorText;
	std::string m_screenResource;
};