#include "GUI/SpellSlot.h"

using namespace std;

SpellSlot::SpellSlot(const SpellBean *spell, const sf::Vector2f &center) : GameObject()
{
	float r = 40.f;
	sf::Vector2f radius(r, r);

	m_spell = spell;
	setBoundingBox(sf::FloatRect(center.x - r, center.y - r, r, r));

	sf::Color color = CENDRIC_COLOR_GREY;
	if (spell->color == SpellColor::Elemental)
		color = CENDRIC_COLOR_ELEMENTAL;
	else if (spell->color == SpellColor::Twilight)
		color = CENDRIC_COLOR_TWILIGHT;
	else if (spell->color == SpellColor::Necromancy)
		color = CENDRIC_COLOR_NECROMANCY;
	else if (spell->color == SpellColor::Divine)
		color = CENDRIC_COLOR_DIVINE;
	else if (spell->color == SpellColor::Illusion)
		color = CENDRIC_COLOR_ILLUSION;

	m_outerRing = sf::CircleShape(r);
	m_outerRing.setPosition(center - sf::Vector2f(radius));
	m_outerRing.setFillColor(CENDRIC_COLOR_WHITE);

	m_coloredRing = sf::CircleShape(0.95f * r);
	m_coloredRing.setPosition(center - 0.95f * radius);
	m_coloredRing.setFillColor(color);

	m_innerRing = sf::CircleShape(0.75f * r);
	m_innerRing.setPosition(center - 0.75f * radius);
	m_innerRing.setFillColor(CENDRIC_COLOR_WHITE);

	// TODO: set texture to icon
	m_inside = sf::CircleShape(0.7f * r);
	m_inside.setPosition(center - 0.7f * radius);
	m_inside.setFillColor(CENDRIC_COLOR_BLACK);

	m_smallRingLeft1 = sf::CircleShape(0.25f * r);
	m_smallRingLeft1.setPosition(center - sf::Vector2f(0.85f * r, 0.f) - 0.25f * radius);
	m_smallRingLeft1.setFillColor(color);
	m_smallRingLeft1.setOutlineColor(CENDRIC_COLOR_WHITE);
	m_smallRingLeft1.setOutlineThickness(0.05f * r);

	m_smallRingLeft2 = sf::CircleShape(0.05f * r);
	m_smallRingLeft2.setPosition(center - sf::Vector2f(0.85f * r, 0.f) - 0.05f * radius);
	m_smallRingLeft2.setFillColor(color);
	m_smallRingLeft2.setOutlineColor(CENDRIC_COLOR_WHITE);
	m_smallRingLeft2.setOutlineThickness(0.05f * r);

	m_smallRingRight1 = m_smallRingLeft1;
	m_smallRingRight1.setPosition(center + sf::Vector2f(0.85f * r, 0.f) - 0.25f * radius);
	m_smallRingRight2 = m_smallRingLeft2;
	m_smallRingRight2.setPosition(center + sf::Vector2f(0.85f * r, 0.f) - 0.05f * radius);

	m_smallRingTop1 = m_smallRingLeft1;
	m_smallRingTop1.setPosition(center - sf::Vector2f(0.f, 0.85f * r) - 0.25f * radius);
	m_smallRingTop2 = m_smallRingLeft2;
	m_smallRingTop2.setPosition(center - sf::Vector2f(0.f, 0.85f * r) - 0.05f * radius);

	m_smallRingBottom1 = m_smallRingLeft1;
	m_smallRingBottom1.setPosition(center + sf::Vector2f(0.f, 0.85f * r) - 0.25f * radius);
	m_smallRingBottom2 = m_smallRingLeft2;
	m_smallRingBottom2.setPosition(center + sf::Vector2f(0.f, 0.85f * r) - 0.05f * radius);
}

void SpellSlot::render(sf::RenderTarget& renderTarget)
{
	renderTarget.draw(m_outerRing);
	renderTarget.draw(m_coloredRing);
	renderTarget.draw(m_innerRing);
	renderTarget.draw(m_inside);

	renderTarget.draw(m_smallRingLeft1);
	renderTarget.draw(m_smallRingLeft2);
	renderTarget.draw(m_smallRingRight1);
	renderTarget.draw(m_smallRingRight2);
	renderTarget.draw(m_smallRingTop1);
	renderTarget.draw(m_smallRingTop2);
	renderTarget.draw(m_smallRingBottom1);
	renderTarget.draw(m_smallRingBottom2);
}

void SpellSlot::update(const sf::Time& frameTime)
{
	// update stuff
	GameObject::update(frameTime);
}

GameObjectType SpellSlot::getConfiguredType() const
{
	return GameObjectType::_Interface;
}