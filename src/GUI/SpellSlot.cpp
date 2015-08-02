#include "GUI/SpellSlot.h"

using namespace std;

const float SpellSlot::RADIUS = 40.f;

SpellSlot::SpellSlot(const sf::Vector2f &center, const SpellBean& spellBean, bool active)
{
	setBoundingBox(sf::FloatRect(center.x - RADIUS, center.y - RADIUS, RADIUS, RADIUS));
	m_spellColor = spellBean.color;

	m_color = CENDRIC_COLOR_GREY;
	m_colorBase = CENDRIC_COLOR_BLACK;
	if (m_spellColor == SpellColor::Elemental)
	{
		m_color = CENDRIC_COLOR_ELEMENTAL;
		m_colorBase = CENDRIC_COLOR_ELEMENTAL_INACTIVE;
		m_textureInactive = g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_elemental);
	}
	else if (m_spellColor == SpellColor::Twilight)
	{
		m_color = CENDRIC_COLOR_TWILIGHT;
		m_colorBase = CENDRIC_COLOR_TWILIGHT_INACTIVE;
		m_textureInactive = g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_twilight);
	}
	else if (m_spellColor == SpellColor::Necromancy)
	{
		m_color = CENDRIC_COLOR_NECROMANCY;
		m_colorBase = CENDRIC_COLOR_NECROMANCY_INACTIVE;
		m_textureInactive = g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_necromancy);
	}
	else if (m_spellColor == SpellColor::Divine)
	{
		m_color = CENDRIC_COLOR_DIVINE;
		m_colorBase = CENDRIC_COLOR_DIVINE_INACTIVE;
		m_textureInactive = g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_divine);
	}
	else if (m_spellColor == SpellColor::Illusion)
	{
		m_color = CENDRIC_COLOR_ILLUSION;
		m_colorBase = CENDRIC_COLOR_ILLUSION_INACTIVE;
		m_textureInactive = g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_illusion);
	}

	m_textureRect = spellBean.iconTextureRect;
	m_isChopSlot = (spellBean.id == SpellID::Chop);

	sf::Vector2f radiusVector(RADIUS, RADIUS);

	m_outerRing = sf::CircleShape(RADIUS);
	m_outerRing.setPosition(center - sf::Vector2f(radiusVector));
	m_outerRing.setFillColor(CENDRIC_COLOR_WHITE);

	m_coloredRingBase = sf::CircleShape(0.95f * RADIUS);
	m_coloredRingBase.setPosition(center - 0.95f * radiusVector);
	m_coloredRingBase.setFillColor(m_colorBase);
    
	m_coloredRing = CircleSector(0.95f * RADIUS);
	m_coloredRing.setPosition(center - 0.95f * radiusVector);
	m_coloredRing.setFillColor(m_color);

	m_innerRing = sf::CircleShape(0.75f * RADIUS);
	m_innerRing.setPosition(center - 0.75f * radiusVector);
	m_innerRing.setFillColor(CENDRIC_COLOR_WHITE);

	m_insideBase = sf::CircleShape(0.7f * RADIUS);
	m_insideBase.setPosition(center - 0.7f * radiusVector);
	m_insideBase.setFillColor(CENDRIC_COLOR_LIGHT_GREY);

	m_inside = sf::CircleShape(0.7f * RADIUS);
	m_inside.setPosition(center - 0.7f * radiusVector);

	m_smallRingLeft1 = sf::CircleShape(0.25f * RADIUS);
	m_smallRingLeft1.setPosition(center - sf::Vector2f(0.85f * RADIUS, 0.f) - 0.25f * radiusVector);
	m_smallRingLeft1.setOutlineColor(CENDRIC_COLOR_WHITE);
	m_smallRingLeft1.setOutlineThickness(0.05f * RADIUS);

	m_smallRingLeft2 = sf::CircleShape(0.05f * RADIUS);
	m_smallRingLeft2.setPosition(center - sf::Vector2f(0.85f * RADIUS, 0.f) - 0.05f * radiusVector);
	m_smallRingLeft2.setOutlineColor(CENDRIC_COLOR_WHITE);
	m_smallRingLeft2.setOutlineThickness(0.05f * RADIUS);

	m_smallRingRight1 = m_smallRingLeft1;
	m_smallRingRight1.setPosition(center + sf::Vector2f(0.85f * RADIUS, 0.f) - 0.25f * radiusVector);
	m_smallRingRight2 = m_smallRingLeft2;
	m_smallRingRight2.setPosition(center + sf::Vector2f(0.85f * RADIUS, 0.f) - 0.05f * radiusVector);

	m_smallRingTop1 = m_smallRingLeft1;
	m_smallRingTop1.setPosition(center - sf::Vector2f(0.f, 0.85f * RADIUS) - 0.25f * radiusVector);
	m_smallRingTop2 = m_smallRingLeft2;
	m_smallRingTop2.setPosition(center - sf::Vector2f(0.f, 0.85f * RADIUS) - 0.05f * radiusVector);

	m_smallRingBottom1 = m_smallRingLeft1;
	m_smallRingBottom1.setPosition(center + sf::Vector2f(0.f, 0.85f * RADIUS) - 0.25f * radiusVector);
	m_smallRingBottom2 = m_smallRingLeft2;
	m_smallRingBottom2.setPosition(center + sf::Vector2f(0.f, 0.85f * RADIUS) - 0.05f * radiusVector);

	if (active)
	{
		activate();
	}
	else
	{
		deactivate();
	}
}

void SpellSlot::activate()
{
	m_active = true;
	m_inside.setTexture(m_isChopSlot ? 
		g_resourceManager->getTexture(ResourceID::Texture_items) : 
		g_resourceManager->getTexture(ResourceID::Texture_spellicons));
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

void SpellSlot::deactivate()
{
	m_active = false;
	m_inside.setTexture(m_textureInactive);
	m_inside.setFillColor(m_colorBase);
	m_coloredRing.setAngle(0.f);

	m_smallRingLeft1.setFillColor(m_colorBase);
	m_smallRingLeft2.setFillColor(m_colorBase);
	m_smallRingRight1.setFillColor(m_colorBase);
	m_smallRingRight2.setFillColor(m_colorBase);
	m_smallRingTop1.setFillColor(m_colorBase);
	m_smallRingTop2.setFillColor(m_colorBase);
	m_smallRingBottom1.setFillColor(m_colorBase);
	m_smallRingBottom2.setFillColor(m_colorBase);
}

void SpellSlot::render(sf::RenderTarget& renderTarget)
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
}

void SpellSlot::playAnimation(const sf::Time &cooldown)
{
	if (m_active)
	{
		m_cooldown = cooldown;
		m_animationTime = sf::Time::Zero;
		m_animating = true;
	}
}

void SpellSlot::update(const sf::Time& frameTime)
{
	float flashTime = 0.05f;	// seconds

	if (m_animating)
	{
		float t = m_animationTime.asSeconds() / m_cooldown.asSeconds();
		float angle = lerp(t, 0.f, 360.f);
		// cout << angle << endl;
		m_coloredRing.setAngle(angle);

		m_animationTime += frameTime;

		if (m_animationTime.asSeconds() > m_cooldown.asSeconds())
		{
			if (m_animationTime.asSeconds() < m_cooldown.asSeconds() + flashTime)
			{
				m_coloredRing.setAngle(360.f);
				m_coloredRing.setFillColor(CENDRIC_COLOR_WHITE);
			}
			else
			{
				m_animationTime = sf::Time::Zero;
				m_coloredRing.setAngle(360.f);
				m_coloredRing.setFillColor(m_color);
				m_animating = false;
			}
			
		}
	}

	GameObject::update(frameTime);
}

GameObjectType SpellSlot::getConfiguredType() const
{
	return GameObjectType::_Interface;
}