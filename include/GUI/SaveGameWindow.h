#pragma once

#include "global.h"
#include "Window.h"

#include "GUI/BitmapText.h"
#include "GUI/SlicedSprite.h"

class ScrollBar;
class ScrollHelper;

// a save game entry in a save game window
class SaveGameEntry : public GameObject {
public:
	SaveGameEntry();

	// loads data from the filename via a character core reader.
	// returns true if successful, else false.
	bool load(const std::string& filename);

	void render(sf::RenderTarget& renderTarget) override;
	GameObjectType getConfiguredType() const override;
	void setPosition(const sf::Vector2f& pos) override;
	void setColor(const sf::Color& color);
	void onLeftClick() override;
	void select();
	void deselect();
	bool isClicked();
	bool isSelected() const;
	const std::string& getFilename() const;
	const std::string getSaveName() const;

private:
	bool m_isSelected = false;
	bool m_isClicked = false;
	BitmapText m_dateSaved;
	BitmapText m_name;
	BitmapText m_timePlayed;

	std::string m_filename;
};

// a window to show savegames in the saves/ folder. 
// it is used inside the SaveGameScreen and the LoadGameScreen
class SaveGameWindow {
public:
	SaveGameWindow();
	virtual ~SaveGameWindow();

	void render(sf::RenderTarget& renderTarget);
	void update(const sf::Time& frameTime);
	// if this is true, the chosen file should be loaded. be aware that this "query" sets this bool to false again.
	bool isChosen();
	void reload();
	void setEnabled(bool value);
	std::string getChosenFilename() const;
	std::string getChosenSaveName() const;

private:
	void calculateEntryPositions();
	void updateScrolling(const sf::Time& frameTime);

public:
	static const float COLUMN_WIDTH;

private:
	std::vector<SaveGameEntry> m_entries;
	int m_chosenEntry = 0;
	bool m_isChosen = false;
	bool m_isEnabled = true;

	// the menu sound
	sf::Sound m_sound;

	SlicedSprite m_scrollWindow;
	ScrollBar* m_scrollBar = nullptr;
	ScrollHelper *m_scrollHelper = nullptr;

	// the time it waits from key active to scrolling
	const sf::Time SCROLL_TIMEOUT = sf::milliseconds(500);
	sf::Time m_upActiveTime = sf::Time::Zero;
	sf::Time m_downActiveTime = sf::Time::Zero;
	// the time between the scroll-ticks, determines the speed of scrolling
	const sf::Time SCROLL_TICK_TIME = sf::milliseconds(70);
	sf::Time m_timeSinceTick = sf::Time::Zero;

	static const int ENTRY_COUNT;

	static const float WINDOW_MARGIN;
	
	static const float TOP;
	static const float LEFT;
	static const float WIDTH;
	static const float HEIGHT;
};