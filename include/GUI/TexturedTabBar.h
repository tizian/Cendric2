#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "GUI/TexturedTabButton.h"

class TexturedTabBar : public GameObject {
public:
	TexturedTabBar(const sf::FloatRect& box, int numberTabs);
	~TexturedTabBar();

	void render(sf::RenderTarget& renderTarget) override;
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& position) override;

	int getActiveTabIndex() const;
	const std::vector<TexturedTabButton*>& getTabButtons() const ;
	TexturedTabButton* getTabButton(int index);

	GameObjectType getConfiguredType() const override;

private:
	int m_activeTabIndex;
	std::vector<TexturedTabButton*> m_tabButtons;
	SlicedSprite m_activeOverlay;
	float m_tabWidth;
};