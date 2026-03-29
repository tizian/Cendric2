#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"

#include "GUI/BitmapText.h"
#include <optional>

class CreditsScreen final : public Screen {
public:
	CreditsScreen(CharacterCore* core);

	void execUpdate(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	void execOnEnter() override;
	void execOnExit() override;

private:
	std::optional<sf::Sprite> m_screenSprite;
	BitmapText* m_title = nullptr;
	BitmapText* m_credits = nullptr;

	void onBack();
};