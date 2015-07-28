#pragma once

#include "global.h"
#include "Window.h"

#include "GUI/BitmapText.h"

// a save game entry in a save game window
class SaveGameEntry : public GameObject
{
public:
	SaveGameEntry();

	// loads data from the filename via a character core reader.
	// returns true if successful, else false.
	bool load(const std::string& filename);
	// gets called after load was successful and sets the entries positon. 
	void init(const sf::Vector2f& position);

	void render(sf::RenderTarget& renderTarget) override;
	GameObjectType getConfiguredType() const override;
	void setPosition(const sf::Vector2f& pos) override;
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
class SaveGameWindow : public Window
{
public:
	SaveGameWindow();
	~SaveGameWindow();
	
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	// if this is true, the chosen file should be loaded. be aware that this "query" sets this bool to false again.
	bool isChosen();
	void reload();
	void setEnabled(bool value);
	std::string getChosenFilename() const;
	std::string getChosenSaveName() const;
	
private:
	std::vector<SaveGameEntry> m_entries;
	int m_chosenEntry = 0;
	bool m_isChosen = false;
	bool m_isEnabled = true;
	
	// <<< scrolling >>>
	// scroll one entry up
	void scrollUp();
	// scroll one entry down
	void scrollDown();
	const int m_maxShowableEntries;
	int m_topEntry = 0;
	int m_bottomEntry = 0;
	// triangle shapes to show that scrolling is possible
	sf::Sprite m_arrowUp;
	sf::Sprite m_arrowDown;
};