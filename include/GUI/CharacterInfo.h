#pragma once

#include "global.h"
#include "Level/LevelMainCharacter.h"
#include "Window.h"
#include "GUI/GUIConstants.h"
#include "GUI/TabBar.h"
#include "Enums/EnumNames.h"

// the character info, as displayed in a world
// it is seperated in two parts, the reputation and the stats.
// for the attributes, it takes them directly from the level main character (level) or the core (map)
class CharacterInfo {
public:
	CharacterInfo(const CharacterCore* core, const AttributeData* attributes);
	~CharacterInfo();

	void show();
	void hide();

	void update(const sf::Time& frameTime);
	void render(sf::RenderTarget& target) const;
	bool isVisible() const;

	// can be called from outside to trigger a reload as soon as the window is visible
	void notifyChange();

private:
	void reload();
	void updateAttributes();
	void updateReputation();

private:
	const CharacterCore* m_core;
	const AttributeData* m_attributes;
	bool m_isVisible = false;
	bool m_isReloadNeeded = true;

	Window* m_window = nullptr;
	TabBar* m_tabBar;
	BitmapText m_title;
	BitmapText m_namesText;
	BitmapText m_attributeText;
	std::vector<BitmapText> m_reputationTexts;
};