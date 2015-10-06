#pragma once

#include "global.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"
#include "TextProvider.h"
#include "Structs/QuestData.h"
#include "CharacterCore.h"
#include "GUI/GUIConstants.h"
#include "GUI/QuestLog.h"

class QuestDescriptionWindow : public Window {
public:
	QuestDescriptionWindow(const CharacterCore* core);

	void reload(const std::string& questID);
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
	BitmapText m_stateText;

	std::vector<BitmapText> m_conditionTexts;
	std::vector<BitmapText> m_targetsTexts;
	std::vector<BitmapText> m_collectiblesTexts;
};