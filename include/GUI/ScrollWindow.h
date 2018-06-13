#pragma once

#include "global.h"
#include "GUI/SlicedSprite.h"
#include "World/GameObject.h"

class ScrollBar;
class ScrollHelper;

// a scroll entry in a scroll window
class ScrollEntry : virtual public GameObject {
public:
	ScrollEntry() = default;
	virtual ~ScrollEntry() = default;

	void update(const sf::Time& frameTime) override;

	void select();
	void deselect();
	void onLeftJustPressed() override;
	void onMouseOver() override;

	bool isSelected() const;
	bool isMouseover() const;
	bool isClicked() const;

	GameObjectType getConfiguredType() const override;

protected:
	virtual void updateColor() = 0;

private:
	bool m_isSelected = false;
	bool m_isClicked = false;
	bool m_isMouseover = false;
};

// a window to show savegames in the saves/ folder. 
// it is used inside the SaveGameScreen and the LoadGameScreen
class ScrollWindow {
public:
	ScrollWindow() = default;
	virtual ~ScrollWindow() = default;

	void selectEntry(int entryId);

protected:
	void calculateEntryPositions();
	void updateSelection(const sf::Time& frameTime);
	virtual void execEntrySelected(const ScrollEntry* entry) {};
	virtual bool isEntryInvisible(const ScrollEntry* entry) const = 0;
	
	virtual int getEntryCount() = 0;
	virtual float getLeft() = 0;
	virtual float getTop() = 0;
	virtual float getWindowMargin() = 0;
	virtual float getWidth() = 0;

protected:
	std::vector<ScrollEntry*>* m_entries = nullptr;
	int m_selectedEntryId = -1;

	SlicedSprite m_scrollWindow;
	ScrollBar* m_scrollBar = nullptr;
	ScrollHelper* m_scrollHelper = nullptr;

private:
	const sf::Time SCROLL_TIMEOUT = sf::milliseconds(500);
	sf::Time m_upActiveTime = sf::Time::Zero;
	sf::Time m_downActiveTime = sf::Time::Zero;
	const sf::Time SCROLL_TICK_TIME = sf::milliseconds(70);
	sf::Time m_timeSinceTick = sf::Time::Zero;
};