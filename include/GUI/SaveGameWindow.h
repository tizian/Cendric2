#pragma once

#include "global.h"
#include "Window.h"

#include "GUI/BitmapText.h"
#include "GUI/ScrollWindow.h"

// a save game entry in a save game window
class SaveGameEntry final : public ScrollEntry {
public:
	SaveGameEntry();

	bool load(const std::string& filename);

	void render(sf::RenderTarget& renderTarget) override;
	void setPosition(const sf::Vector2f& pos) override;
	
	const std::string& getFilename() const;
	const std::string& getSaveName() const;

	bool operator> (const SaveGameEntry& save) const {
		return (m_dateSavedNr > save.m_dateSavedNr);
	}

protected:
	void updateColor() override;

private:
	void setColor(const sf::Color& color);

	bool m_isHashValid = false;
	BitmapText m_dateSaved;
	BitmapText m_name;
	BitmapText m_timePlayed;

	std::string m_filename;
	std::time_t m_dateSavedNr;
};

// a window to show savegames in the saves/ folder. 
// it is used inside the SaveGameScreen and the LoadGameScreen
class SaveGameWindow : public ScrollWindow {
public:
	SaveGameWindow();
	virtual ~SaveGameWindow();

	void render(sf::RenderTarget& renderTarget);
	void update(const sf::Time& frameTime);

	bool isChosen();
	void reload();
	void setEnabled(bool value);
	std::string getChosenFilename() const;
	std::string getChosenSaveName() const;

	// fills the given vector with saves and orders them (newest first)
	static void loadSaves(std::vector<ScrollEntry*>& entries);

public:
	static const float COLUMN_WIDTH;

protected:
	bool isEntryInvisible(const ScrollEntry* entry) const override;

	int getEntryCount() override { return ENTRY_COUNT; }
	float getLeft() override { return LEFT; }
	float getTop() override { return TOP; }
	float getWindowMargin() override { return WINDOW_MARGIN; }
	float getWidth() override { return WIDTH; }

private:
	std::vector<ScrollEntry*> m_saveGameEntries;
	bool m_isChosen = false;
	bool m_isEnabled = true;

	BitmapText m_emptyText;

	static const int ENTRY_COUNT;
	static const float WINDOW_MARGIN;
	
	static const float TOP;
	static const float LEFT;
	static const float WIDTH;
	static const float HEIGHT;
};