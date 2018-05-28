#pragma once

#include "global.h"
#include "World/GameObject.h"
#include "GUI/TabButton.h"

class TabBar : virtual public GameObject {
public:
	TabBar() = default;
	virtual ~TabBar();

	virtual void init(const sf::FloatRect& box, int numberTabs);

	void render(sf::RenderTarget& renderTarget) override;
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& position) override;
	void setGamepadEnabled(bool enabled);

	int getActiveTabIndex() const;
	std::vector<TabButton*>& getTabButtons();
	virtual TabButton* getTabButton(int index) const;
	
	GameObjectType getConfiguredType() const override;

protected:
	int m_activeTabIndex;
	float m_tabWidth;
	bool m_isGamepadEnabled;
	std::vector<TabButton*> m_tabButtons;
	SlicedSprite m_activeOverlay;
};