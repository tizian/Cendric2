#pragma once

#include "global.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"
#include "GUI/GUIConstants.h"
#include "Structs/NPCData.h"
#include "Screens/WorldScreen.h"
#include "Map/MerchantInterface.h"
#include "Map/Dialogue.h"

class CharacterCore;
class ScrollBar;
class ScrollHelper;

class CookingOption : public virtual GameObject {
public:
	CookingOption(const std::string& itemID, const std::string& cookedItemID, int count, int nr);
	void render(sf::RenderTarget& renderTarget) override;
	GameObjectType getConfiguredType() const override;
	void setPosition(const sf::Vector2f& pos) override;
	void setColor(const sf::Color& color);
	void onLeftClick() override;
	void select();
	void deselect();
	// query with side effects (unclicks automatically) (bertrand would kill me)
	bool isClicked();
	bool isSelected() const;
	const std::string& getItemID() const;
private:
	bool m_isSelected = false;
	bool m_isClicked = false;
	std::string m_itemID;
	sf::Text m_text;
};

class MapScreen;

class CookingWindow : public Window {
public:
	CookingWindow(MapScreen* screen);
	~CookingWindow();
	
	void render(sf::RenderTarget& renderTarget) override;
	// returns true as long as the dialogue exists and false as soon as it ends
	bool updateWindow(const sf::Time frameTime);
	
	void setPosition(const sf::Vector2f& pos) override;

private:
	void reload();
	void cookItem(const std::string& itemID);

	void calculateEntryPositions();
	void updateScrolling(const sf::Time& frameTime);

private:
	std::vector<CookingOption> m_options;
	int m_chosenOption = 0;

	SlicedSprite m_scrollWindow;
	ScrollBar* m_scrollBar = nullptr;
	ScrollHelper *m_scrollHelper = nullptr;

	sf::Sprite m_cookingSprite;
	BitmapText m_title;
	
	MapScreen* m_screen = nullptr;
	
	// the time it waits from key active to scrolling
	const sf::Time SCROLL_TIMEOUT = sf::milliseconds(500);
	sf::Time m_upActiveTime = sf::Time::Zero;
	sf::Time m_downActiveTime = sf::Time::Zero;
	// the time between the scroll-ticks, determines the speed of scrolling
	const sf::Time SCROLL_TICK_TIME = sf::milliseconds(70);
	sf::Time m_timeSinceTick = sf::Time::Zero;

	static const int OPTION_COUNT;

	static const float LEFT_OFFSET;
	static const float RIGHT_OFFSET;

	static const float TEXT_WIDTH;

	static const float WINDOW_MARGIN;

	static const float TOP;
	static const float LEFT;
	static const float WIDTH;
	static const float HEIGHT;

	static const float SCROLL_WINDOW_TOP;
	static const float SCROLL_WINDOW_LEFT;
	static const float SCROLL_WINDOW_WIDTH;
	static const float SCROLL_WINDOW_HEIGHT;
};