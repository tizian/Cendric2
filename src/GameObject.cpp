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
	if (DEBUG_RENDERING && getConfiguredDebugColor() != sf::Color::Transparent)
	{
		drawBoundingBox(renderTarget);
	}
}

void GameObject::drawBoundingBox(sf::RenderTarget& renderTarget) const
{
	sf::RectangleShape rectangle(sf::Vector2f(m_boundingBox.width, m_boundingBox.height));
	rectangle.setPosition(m_position);
	rectangle.setOutlineThickness(1.f);
	rectangle.setFillColor(sf::Color::Transparent);
	rectangle.setOutlineColor(getConfiguredDebugColor());
	renderTarget.draw(rectangle);
}

sf::Color GameObject::getConfiguredDebugColor() const
{
	return sf::Color::Transparent;
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
	else if (g_inputController->isRightJustPressed(&m_boundingBox))
	{
		onRightJustPressed();
	}
	if (g_inputController->isLeftClicked(&m_boundingBox))
	{
		onLeftClick();
	}
	else if (g_inputController->isLeftJustPressed(&m_boundingBox))
	{
		onLeftJustPressed();
	}
	m_animatedSprite.update(frameTime);
}

void GameObject::setPosition(const sf::Vector2f& position)
{
	m_position = position;
	m_boundingBox.left = position.x;
	m_boundingBox.top = position.y;
	// origin is set to the center for rotation + flipping
	sf::Vector2f spriteCenter(sf::Vector2f(m_boundingBox.width / 2, m_boundingBox.height / 2) - m_spriteOffset);
	m_animatedSprite.setOrigin(spriteCenter);
	m_animatedSprite.setPosition(position + m_spriteOffset + spriteCenter);
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

void GameObject::setRotation(float angle)
{
	// SFML uses degrees, not radian, hence the conversion here. 
	sf::Vector2f oldOrigin = m_animatedSprite.getOrigin();
	
	m_animatedSprite.setRotation(angle * (180.f / PI_F));

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

void GameObject::onRightJustPressed()
{
	// nop
}

void GameObject::onLeftJustPressed()
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
