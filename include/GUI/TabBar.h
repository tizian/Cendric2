#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "GUI/TabButton.h"

class TabBar : public GameObject {
public:
	TabBar(const sf::FloatRect& box, int numberTabs);
	~TabBar();

	void render(sf::RenderTarget& renderTarget) override;
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	int getActiveTabIndex() const;
	std::vector<TabButton*>& getTabButtons();
	TabButton* getTabButton(int index);
	
	GameObjectType getConfiguredType() const override;

private:
	int m_activeTabIndex;
	std::vector<TabButton*> m_tabButtons;
	SlicedSprite m_activeOverlay;
};