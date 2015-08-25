#include "GUI/BuffSlot.h"

using namespace std;

const float BuffSlot::RADIUS = 25.f;
const float BuffSlot::MARGIN = 5.f;
const sf::Time BuffSlot::FLASHING_TIME = sf::seconds(5);
const sf::Time BuffSlot::FLASHING_INTERVAL = sf::seconds(0.5);

BuffSlot::BuffSlot(BuffType type, const sf::IntRect& textureLocation, const sf::Time& duration)
{
	setBoundingBox(sf::FloatRect(0.f, 0.f, RADIUS + MARGIN, RADIUS + MARGIN));
	
	m_duration = duration;
	m_timeUntilFlash = std::max(FLASHING_INTERVAL, duration - FLASHING_TIME);
	m_isVisible = true;

	sf::Texture* texture;

	m_outerRing = sf::CircleShape(RADIUS + MARGIN);

	switch (type)
	{
	case BuffType::Food:
		m_outerRing.setFillColor(sf::Color::Green);
		texture = g_resourceManager->getTexture(ResourceID::Texture_items);
		break;
	case BuffType::Spell:
		m_outerRing.setFillColor(CENDRIC_COLOR_DIVINE);
		texture = g_resourceManager->getTexture(ResourceID::Texture_spellicons);
		break;
	default:
		break;
	}

	m_innerRing = sf::CircleShape(RADIUS);
	m_innerRing.setFillColor(CENDRIC_COLOR_LIGHT_GREY);
	m_inside = sf::CircleShape(RADIUS);
	m_inside.setTexture(texture);
	m_inside.setTextureRect(textureLocation);

	m_durationText.setCharacterSize(16);
}

void BuffSlot::setPosition(const sf::Vector2f& pos)
{
	m_outerRing.setPosition(pos);
	m_innerRing.setPosition(pos + sf::Vector2f(MARGIN, MARGIN));
	m_inside.setPosition(pos + sf::Vector2f(MARGIN, MARGIN));
	m_durationText.setPosition(pos + sf::Vector2f(0.f, 3 * MARGIN + 2 * RADIUS));
	GameObject::setPosition(pos);
}

void BuffSlot::render(sf::RenderTarget& renderTarget)
{
	if (m_isVisible)
	{
		renderTarget.draw(m_outerRing);
		renderTarget.draw(m_innerRing);
		renderTarget.draw(m_inside);	
	}

	renderTarget.draw(m_durationText);
}

void BuffSlot::update(const sf::Time& frameTime)
{
	// update time
	GameObject::updateTime(m_duration, frameTime);
	if (m_duration == sf::Time::Zero) setDisposed();
	GameObject::updateTime(m_timeUntilFlash, frameTime);
	if (m_timeUntilFlash == sf::Time::Zero)
	{
		m_isVisible = !m_isVisible;
		m_timeUntilFlash = FLASHING_INTERVAL;
	}
	
	// update duration text
	m_durationText.setString(to_string(static_cast<int>(floor(m_duration.asSeconds()))) + "s");
	GameObject::update(frameTime);
}

GameObjectType BuffSlot::getConfiguredType() const
{
	return GameObjectType::_Interface;
}