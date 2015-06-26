#include "NPC.h"
#include "MapMainCharacter.h"

void NPC::load(MapMainCharacter* mainChar, NpcID id)
{
	m_mainChar = mainChar;
	m_npcID = id;
	setDebugBoundingBox(sf::Color::Magenta);
}

void NPC::onMouseOver()
{
	m_tooltipTime = sf::seconds(1);
}

void NPC::render(sf::RenderTarget &renderTarget)
{
	GameObject::render(renderTarget);
	if (m_tooltipTime > sf::Time::Zero)
	{
		renderTarget.draw(m_tooltipText);
	}
}

void NPC::update(const sf::Time& frameTime)
{
	GameObject::update(frameTime);
	if (m_tooltipTime > sf::Time::Zero)
	{
		m_tooltipTime = m_tooltipTime - frameTime;
		if (m_tooltipTime < sf::Time::Zero)
		{
			m_tooltipTime = sf::Time::Zero;
		}
	}
}

GameObjectType NPC::getConfiguredType() const
{
	return GameObjectType::_NPC;
}

void NPC::setTooltipText(const std::wstring& tooltip)
{
	m_tooltipText = BitmapText(tooltip);
	m_tooltipText.setColor(sf::Color::White);
	m_tooltipText.setCharacterSize(8);
	m_tooltipText.setPosition(sf::Vector2f(getPosition().x, getPosition().y - 10.f));
}
