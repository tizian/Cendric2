#pragma once

#include "global.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"
#include "TextProvider.h"
#include "CharacterCore.h"
#include "GUI/GUIConstants.h"
#include "GUI/CharacterInfo.h"
#include "GUI/Hints.h"

class HintDescriptionWindow final : public Window {
public:
	HintDescriptionWindow(const CharacterCore* core);

	void reload(const std::string& hintKey);
	void render(sf::RenderTarget& renderTarget) override;
	void setPosition(const sf::Vector2f& position) override;

	void show();
	void hide();

	static const float WIDTH;

private:
	const CharacterCore* m_core;

	bool m_isVisible = false;

	BitmapText m_titleText;
	BitmapText m_descriptionText;
};