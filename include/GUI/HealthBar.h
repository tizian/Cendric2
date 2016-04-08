#pragma once

#include "global.h"
#include "Structs/AttributeData.h"

// the health bar, as displayed in a level. It takes its data from the attributes of the main character.
class HealthBar {
public:
	HealthBar(const AttributeData* attributes);

	void render(sf::RenderTarget& target) const;
	void update(const sf::Time& frameTime);

private:
	const AttributeData* m_attributes;

	int m_currentHP;
	int m_maxOverlayHP;
	int m_overlayHP;

	sf::Time m_highlightTime;
	sf::Time m_waitTime;
	sf::Time m_shrinkTime;

	sf::RectangleShape m_border;
	sf::RectangleShape m_bar;
	sf::RectangleShape m_hitOverlay;

	const sf::Texture *m_borderTexture;
	const sf::Texture *m_barTexture;
	const sf::Texture *m_hitOverlayTexture;
	const sf::Texture *m_hitOverlayHighlightTexture;

	const float BAR_HEIGHT = 15.f;
	const float BAR_WIDTH = 300.f;
	const float BAR_LEFT = 30.f;
	const float BAR_TOP = 30.f;

	const float BORDER_OFFSET_X = 13.f;
	const float BORDER_OFFSET_Y = 10.f;

	const float HIGHLIGHT_TIME = 0.05f;
	const float WAIT_TIME = 1.f;
	const float SHRINK_TIME = 1.f;
};