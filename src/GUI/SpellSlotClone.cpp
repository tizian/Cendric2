#include "GUI/SpellSlotClone.h"

using namespace std;

SpellSlotClone::SpellSlotClone(const SpellSlot* original)
{
	m_original = original;
	m_spellType = original->getSpellType();
	m_spellID = original->getSpellID();
	m_textureRect = original->getTextureRect();

	init();
}

void SpellSlotClone::init()
{
	setBoundingBox(sf::FloatRect(0.f, 0.f, 2 * SpellSlot::RADIUS, 2 * SpellSlot::RADIUS));
	setDebugBoundingBox(sf::Color::Red);
	setInputInDefaultView(true);

	m_color = CENDRIC_COLOR_GREY;
	m_colorBase = CENDRIC_COLOR_BLACK;
	if (m_spellType == SpellType::Elemental)
	{
		m_color = CENDRIC_COLOR_ELEMENTAL;
		m_colorBase = CENDRIC_COLOR_ELEMENTAL_INACTIVE;
	}
	else if (m_spellType == SpellType::Twilight)
	{
		m_color = CENDRIC_COLOR_TWILIGHT;
		m_colorBase = CENDRIC_COLOR_TWILIGHT_INACTIVE;
	}
	else if (m_spellType == SpellType::Necromancy)
	{
		m_color = CENDRIC_COLOR_NECROMANCY;
		m_colorBase = CENDRIC_COLOR_NECROMANCY_INACTIVE;
	}
	else if (m_spellType == SpellType::Divine)
	{
		m_color = CENDRIC_COLOR_DIVINE;
		m_colorBase = CENDRIC_COLOR_DIVINE_INACTIVE;
	}
	else if (m_spellType == SpellType::Illusion)
	{
		m_color = CENDRIC_COLOR_ILLUSION;
		m_colorBase = CENDRIC_COLOR_ILLUSION_INACTIVE;
	}

	m_outerRing = sf::CircleShape(SpellSlot::RADIUS);
	m_outerRing.setFillColor(CENDRIC_COLOR_WHITE);

	m_coloredRingBase = sf::CircleShape(0.95f * SpellSlot::RADIUS);
	m_coloredRingBase.setFillColor(m_colorBase);

	m_coloredRing = CircleSector(0.95f * SpellSlot::RADIUS);
	m_coloredRing.setFillColor(m_color);

	m_innerRing = sf::CircleShape(0.75f * SpellSlot::RADIUS);
	m_innerRing.setFillColor(CENDRIC_COLOR_WHITE);

	m_insideBase = sf::CircleShape(0.7f * SpellSlot::RADIUS);
	m_insideBase.setFillColor(CENDRIC_COLOR_LIGHT_GREY);

	m_inside = sf::CircleShape(0.7f * SpellSlot::RADIUS);

	m_smallRingLeft1 = sf::CircleShape(0.25f * SpellSlot::RADIUS);
	m_smallRingLeft1.setOutlineColor(CENDRIC_COLOR_WHITE);
	m_smallRingLeft1.setOutlineThickness(0.05f * SpellSlot::RADIUS);

	m_smallRingLeft2 = sf::CircleShape(0.05f * SpellSlot::RADIUS);
	m_smallRingLeft2.setOutlineColor(CENDRIC_COLOR_WHITE);
	m_smallRingLeft2.setOutlineThickness(0.05f * SpellSlot::RADIUS);

	m_smallRingRight1 = m_smallRingLeft1;
	m_smallRingRight2 = m_smallRingLeft2;

	m_smallRingTop1 = m_smallRingLeft1;
	m_smallRingTop2 = m_smallRingLeft2;

	m_smallRingBottom1 = m_smallRingLeft1;
	m_smallRingBottom2 = m_smallRingLeft2;

	m_inside.setTexture(g_resourceManager->getTexture(ResourceID::Texture_spellicons));
	m_inside.setTextureRect(m_textureRect);
	m_inside.setFillColor(CENDRIC_COLOR_WHITE);
	m_coloredRing.setAngle(360.f);

	m_smallRingLeft1.setFillColor(m_color);
	m_smallRingLeft2.setFillColor(m_color);
	m_smallRingRight1.setFillColor(m_color);
	m_smallRingRight2.setFillColor(m_color);
	m_smallRingTop1.setFillColor(m_color);
	m_smallRingTop2.setFillColor(m_color);
	m_smallRingBottom1.setFillColor(m_color);
	m_smallRingBottom2.setFillColor(m_color);
}

void SpellSlotClone::setPosition(const sf::Vector2f& pos)
{
	m_position = pos;
	m_boundingBox.left = pos.x - SpellSlot::RADIUS;
	m_boundingBox.top = pos.y - SpellSlot::RADIUS;

	sf::Vector2f radiusVector(SpellSlot::RADIUS, SpellSlot::RADIUS);
	m_debugBox.setPosition(pos - radiusVector);

	m_outerRing.setPosition(pos - sf::Vector2f(radiusVector));
	m_coloredRingBase.setPosition(pos - 0.95f * radiusVector);
	m_coloredRing.setPosition(pos - 0.95f * radiusVector);
	m_innerRing.setPosition(pos - 0.75f * radiusVector);
	m_insideBase.setPosition(pos - 0.7f * radiusVector);
	m_inside.setPosition(pos - 0.7f * radiusVector);

	m_smallRingLeft1.setPosition(pos - sf::Vector2f(0.85f * SpellSlot::RADIUS, 0.f) - 0.25f * radiusVector);
	m_smallRingLeft2.setPosition(pos - sf::Vector2f(0.85f * SpellSlot::RADIUS, 0.f) - 0.05f * radiusVector);
	m_smallRingRight1.setPosition(pos + sf::Vector2f(0.85f * SpellSlot::RADIUS, 0.f) - 0.25f * radiusVector);
	m_smallRingRight2.setPosition(pos + sf::Vector2f(0.85f * SpellSlot::RADIUS, 0.f) - 0.05f * radiusVector);
	m_smallRingTop1.setPosition(pos - sf::Vector2f(0.f, 0.85f * SpellSlot::RADIUS) - 0.25f * radiusVector);
	m_smallRingTop2.setPosition(pos - sf::Vector2f(0.f, 0.85f * SpellSlot::RADIUS) - 0.05f * radiusVector);
	m_smallRingBottom1.setPosition(pos + sf::Vector2f(0.f, 0.85f * SpellSlot::RADIUS) - 0.25f * radiusVector);
	m_smallRingBottom2.setPosition(pos + sf::Vector2f(0.f, 0.85f * SpellSlot::RADIUS) - 0.05f * radiusVector);
}

void SpellSlotClone::render(sf::RenderTarget& renderTarget)
{
	renderTarget.draw(m_outerRing);
	renderTarget.draw(m_coloredRingBase);
	renderTarget.draw(m_coloredRing);
	renderTarget.draw(m_innerRing);
	renderTarget.draw(m_insideBase);
	renderTarget.draw(m_inside);

	renderTarget.draw(m_smallRingLeft1);
	renderTarget.draw(m_smallRingLeft2);
	renderTarget.draw(m_smallRingRight1);
	renderTarget.draw(m_smallRingRight2);
	renderTarget.draw(m_smallRingTop1);
	renderTarget.draw(m_smallRingTop2);
	renderTarget.draw(m_smallRingBottom1);
	renderTarget.draw(m_smallRingBottom2);

	GameObject::renderAfterForeground(renderTarget);
}

GameObjectType SpellSlotClone::getConfiguredType() const
{
	return GameObjectType::_Interface;
}

const SpellSlot* SpellSlotClone::getOriginal() const
{
	return m_original;
}