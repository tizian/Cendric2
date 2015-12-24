#pragma once

#include "global.h"
#include "Structs/AttributeData.h"

// the health bar, as displayed in a level. It takes its data from the attributes of the main character.
class HealthBar {
public:
	HealthBar(const AttributeData* attributes);

	void render(sf::RenderTarget& target) const;
	void update();

private:
	const AttributeData* m_attributes;

	sf::RectangleShape m_barOutline;
	sf::RectangleShape m_bar;
	const float BAR_HEIGHT = 10.f;
	const float BAR_WIDTH = 200.f;
	const float BAR_OUTLINE_THICKNESS = 2.f;
	const float BAR_LEFT = 10.f;
	const float BAR_TOP = 10.f;
};