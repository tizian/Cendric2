#pragma once

#include "global.h"
#include "Level/LevelMainCharacter.h"
#include "Window.h"
#include "GUI/GUIConstants.h"
#include "GUI/TabBar.h"
#include "Enums/EnumNames.h"

class HintDescriptionWindow;
class ScrollBar;
class ScrollHelper;
class WorldScreen;

// a hint entry in the hints tab
class HintEntry final : virtual public GameObject {
public:
	HintEntry(const std::string& hintKey);

	void render(sf::RenderTarget& renderTarget) override;
	GameObjectType getConfiguredType() const override;
	void setPosition(const sf::Vector2f& pos) override;
	void onLeftJustPressed() override;
	void setColor(const sf::Color& color);
	void select();
	void deselect();
	bool isClicked();
	bool isSelected() const;
	const std::string& getHintKey() const;

private:
	bool m_isSelected = false;
	bool m_isClicked = false;
	BitmapText m_name;

	std::string m_hintKey;
};

// the character info, as displayed in a world
// it is seperated in two parts, the reputation and the stats.
// for the attributes, it takes them directly from the level main character (level) or the core (map)
class CharacterInfo {
public:
	CharacterInfo(WorldScreen* screen, const AttributeData* attributes);
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

	void selectEntry(HintEntry* selectedEntry);
	void calculateEntryPositions();

	void showDescription(const std::string& hintKey);
	void hideDescription();

public:
	static const int MAX_ENTRY_LENGTH_CHARACTERS;

	static const float TOP;
	static const float LEFT;
	static const float WIDTH;
	static const float HEIGHT;

private:
	WorldScreen* m_screen;
	const CharacterCore* m_core;
	const AttributeData* m_attributes;
	bool m_isVisible = false;
	bool m_isReloadNeeded = true;

	Window* m_window = nullptr;
	TabBar* m_tabBar;
	BitmapText m_title;

	std::vector<sf::Sprite> m_statIcons;
	std::vector<BitmapText> m_nameTexts;
	std::vector<BitmapText> m_attributeTexts;

	BitmapText m_guild;
	sf::Sprite m_guildSprite;
	std::vector<BitmapText> m_reputationTexts;

	std::vector<HintEntry> m_hintEntries;
	HintEntry* m_selectedEntry = nullptr;
	std::string m_selectedHintKey = "";

	SlicedSprite m_scrollWindow;
	ScrollBar* m_scrollBar = nullptr;
	ScrollHelper* m_scrollHelper = nullptr;

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