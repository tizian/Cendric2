#include "GameObject.h"

void GameObject::setCurrentAnimation(const Animation* animation, bool isFlipped)
{
	m_animatedSprite.setFlipped(isFlipped);
	m_animatedSprite.setAnimation(animation);
}

void GameObject::addAnimation(GameObjectState state, Animation& animation)
{
	m_animations.insert({ state, animation });
}

const Animation* GameObject::getAnimation(GameObjectState state)
{
	return &m_animations[state];
}

void GameObject::render(sf::RenderTarget &renderTarget)
{
	renderTarget.draw(m_animatedSprite);
}

void GameObject::update(const sf::Time& frameTime)
{
	if (g_inputController->isMouseOver(&m_boundingBox))
	{
		onMouseOver();
	}
	if (g_inputController->isRightClicked(&m_boundingBox))
	{
		onRightClick();
	}
	if (g_inputController->isLeftClicked(&m_boundingBox))
	{
		onLeftClick();
	}
	m_animatedSprite.update(frameTime);
}

void GameObject::setPosition(const sf::Vector2f& position)
{
	m_position = position;
	m_boundingBox.left = position.x;
	m_boundingBox.top = position.y;
	m_animatedSprite.setPosition(position + m_spriteOffset);
}

void GameObject::setPositionX(const float posX)
{
	const sf::Vector2f newPosition(posX, m_position.y);
	setPosition(newPosition);
}

void GameObject::setPositionY(const float posY)
{
	const sf::Vector2f newPosition(m_position.x, posY);
	setPosition(newPosition);
}

void GameObject::setFrameTime(const sf::Time& time) 
{
	m_animatedSprite.setFrameTime(time);
}

void GameObject::playCurrentAnimation(bool play)
{
	if (play)
	{
		m_animatedSprite.play();
	}
	else 
	{
		m_animatedSprite.pause();
	}
}

void GameObject::loopCurrentAnimation(bool loop)
{
	m_animatedSprite.setLooped(loop);
}

void GameObject::setSpriteOffset(const sf::Vector2f& spriteOffset)
{
	m_spriteOffset = spriteOffset;
}

const sf::Vector2f& GameObject::getSpriteOffset() const
{
	return m_spriteOffset;
}

void GameObject::setBoundingBox(const sf::FloatRect& rect)
{
	m_boundingBox = rect;
}

const sf::FloatRect* GameObject::getBoundingBox() const
{
	return &m_boundingBox;
}

void GameObject::onMouseOver() 
{
	// nop
}

void GameObject::onRightClick()
{
	// nop
}

void GameObject::onLeftClick()
{
	// nop
}

const sf::Vector2f GameObject::getCenter() const
{
	return sf::Vector2f(
		m_boundingBox.left + (m_boundingBox.width / 2),
		m_boundingBox.top + (m_boundingBox.height / 2));
}

const sf::Vector2f& GameObject::getPosition() const
{
	return m_position;
}

bool GameObject::isDisposed() const
{
	return m_isDisposed;
}

void GameObject::setDisposed()
{
	m_isDisposed = true;
}

void GameObject::setScreen(Screen* screen) 
{
	m_screen = screen;
}
