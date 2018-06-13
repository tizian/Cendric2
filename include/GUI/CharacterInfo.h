#pragma once

#include "global.h"
#include "Level/LevelMainCharacter.h"
#include "GUI/TabBar.h"
#include "GUI/SelectableWindow.h"
#include "GUI/ScrollWindow.h"

class HintDescriptionWindow;
class WorldScreen;

// a hint entry in the hints tab
class HintEntry final : public ScrollEntry {
public:
	HintEntry(const std::string& hintKey);

	void render(sf::RenderTarget& renderTarget) override;
	void setPosition(const sf::Vector2f& pos) override;
	void setColor(const sf::Color& color);
	const std::string& getHintKey() const;

protected:
	void updateColor() override;

private:
	BitmapText m_name;
	std::string m_hintKey;
};

// the character info, as displayed in a world
// it is seperated in two parts, the reputation and the stats.
// for the attributes, it takes them directly from the level main character (level) or the core (map)
class CharacterInfo : public SelectableWindow, public ScrollWindow {
public:
	CharacterInfo(WorldScreen* screen, const AttributeData* attributes = nullptr);
	~CharacterInfo();

	void show();
	void hide();

	void update(const sf::Time& frameTime);
	void render(sf::RenderTarget& target);
	bool isVisible() const;

	// can be called from outside to trigger a reload as soon as the window is visible
	void notifyChange();

private:
	void reload();
	void updateAttributes();
	void updateReputation();
	void updateHints();

	void showDescription(const std::string& hintKey);
	void hideDescription();

protected:
	void updateWindowSelected() override;
	bool isEntryInvisible(const ScrollEntry* entry) const override;
	void execEntrySelected(const ScrollEntry* entry) override;

	int getEntryCount() override { return ENTRY_COUNT; }
	float getLeft() override { return LEFT + SCROLL_WINDOW_LEFT; }
	float getTop() override { return TOP + SCROLL_WINDOW_TOP; }
	float getWindowMargin() override { return WINDOW_MARGIN; }
	float getWidth() override { return SCROLL_WINDOW_WIDTH; }

public:
	static const int MAX_ENTRY_LENGTH_CHARACTERS;

	static const float TOP;
	static const float LEFT;
	static const float WIDTH;
	static const float HEIGHT;

private:
	void updateTabBar(const sf::Time& frameTime);
	void updateSelectableWindow();
	void checkReload();

private:
	WorldScreen* m_screen;
	const CharacterCore* m_core;
	const AttributeData* m_attributes;
	bool m_isVisible = false;
	bool m_isReloadNeeded = true;

	TabBar* m_tabBar;
	BitmapText m_title;

	std::vector<sf::Sprite> m_statIcons;
	std::vector<BitmapText> m_nameTexts;
	std::vector<BitmapText> m_attributeTexts;

	BitmapText m_guild;
	sf::Sprite m_guildSprite;
	std::vector<BitmapText> m_reputationTexts;

	std::vector<ScrollEntry*> m_hintEntries;
	std::string m_selectedHintKey = "";

	HintDescriptionWindow* m_descriptionWindow = nullptr;

	static const int ENTRY_COUNT;
	static const float MAX_ENTRY_LENGTH;

	static const float WINDOW_MARGIN;

	static const sf::Vector2f BUTTON_SIZE;

	static const float SCROLL_WINDOW_LEFT;
	static const float SCROLL_WINDOW_TOP;
	static const float SCROLL_WINDOW_WIDTH;
	static const float SCROLL_WINDOW_HEIGHT;

	static const std::vector<std::string> LABELS;
};