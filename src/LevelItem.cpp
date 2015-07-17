#include "LevelItem.h"
#include "LevelMainCharacter.h"

void LevelItem::load(LevelMainCharacter* mainChar, const ItemBean& bean, const sf::Vector2f& position)
{
	m_mainChar = mainChar;
	m_itemID = bean.id;

	Animation idleAnimation;
	setSpriteOffset(bean.spriteOffset);
	setBoundingBox(bean.boundingBox);
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_levelitems));
	// add frames
	for (auto &frame : bean.texturePositions) {
		idleAnimation.addFrame(frame);
	}
	addAnimation(GameObjectState::Idle, idleAnimation);
	setFrameTime(bean.frameTime);
	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(bean.texturePositions.size() > 1);

	setPosition(position - getSpriteOffset());
	setTooltipText(g_textProvider->getText(bean.id));
	setDebugBoundingBox(sf::Color::Green);
}

void LevelItem::onRightClick()
{
	// check if item is in range
	sf::Vector2f dist = m_mainChar->getCenter() - getCenter();
	if (sqrt(dist.x * dist.x + dist.y * dist.y) <= PICKUP_RANGE)
	{
		// pickup, create the correct item or correct amount of gold in the players inventory.
		if (m_itemType == ItemType::Gold)
		{
			m_mainChar->addGold(m_goldValue);
		}
		else
		{
			m_mainChar->lootItem(m_itemID, 1);
		}
		setDisposed();
	}
	else
	{
		m_screen->setTooltipText(g_textProvider->getText("OutOfRange"), sf::Color::Red, true);
	}
}

void LevelItem::onInteractKey()
{
	// it is made impossible for a game object to have onInteractKey and onRightClick called in the same game loop
	onRightClick();
}

void LevelItem::onMouseOver()
{
	m_animatedSprite.setColor(sf::Color::Red);
	m_tooltipTime = sf::seconds(1);
}

void LevelItem::render(sf::RenderTarget &renderTarget)
{
	GameObject::render(renderTarget);
	m_animatedSprite.setColor(sf::Color::White);
}

void LevelItem::renderAfterForeground(sf::RenderTarget& renderTarget)
{
	GameObject::renderAfterForeground(renderTarget);
	if (m_tooltipTime > sf::Time::Zero)
	{
		renderTarget.draw(m_tooltipText);
	}
}

void LevelItem::update(const sf::Time& frameTime)
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

GameObjectType LevelItem::getConfiguredType() const
{
	return GameObjectType::_LevelItem;
}

void LevelItem::setTooltipText(const std::wstring& tooltip)
{
	m_tooltipText = BitmapText(tooltip);
	m_tooltipText.setColor(sf::Color::White);
	m_tooltipText.setCharacterSize(8);
	m_tooltipText.setPosition(sf::Vector2f(getPosition().x, getPosition().y - 10.f));
}

