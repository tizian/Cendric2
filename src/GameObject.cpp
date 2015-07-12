#include "GameObject.h"

GameObject::GameObject()
{
	m_boundingBoxes = std::vector<sf::FloatRect>();
	m_boundingBoxes.push_back(sf::FloatRect());
}

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

void GameObject::renderAfterForeground(sf::RenderTarget &renderTarget)
{
	if (DEBUG_RENDERING && m_isDrawBoundingBox)
	{
		renderTarget.draw(m_debugBox);
	}
}

void  GameObject::setDebugBoundingBox(sf::Color debugColor)
{
	// TODO: maybe extend for multiple BBs
	m_debugBox = sf::RectangleShape(sf::Vector2f(m_boundingBoxes[0].width, m_boundingBoxes[0].height));
	m_debugBox.setPosition(m_position);
	m_debugBox.setOutlineThickness(1.f);
	m_debugBox.setFillColor(sf::Color::Transparent);
	m_debugBox.setOutlineColor(debugColor);
	m_isDrawBoundingBox = true;
}

void GameObject::update(const sf::Time& frameTime)
{
	// TODO: maybe extend for multiple BBs
	if (g_inputController->isMouseOver(&m_boundingBoxes[0]))
	{
		onMouseOver();
		if (g_inputController->isKeyJustPressed(Key::Interact))
		{
			onInteractKey();
		}
		else if (g_inputController->isRightClicked(&m_boundingBoxes[0]))
		{
			onRightClick();
		}
		else if (g_inputController->isRightJustPressed(&m_boundingBoxes[0]))
		{
				onRightJustPressed();
		}
		else if (g_inputController->isLeftClicked(&m_boundingBoxes[0]))
		{
			onLeftClick();
		}
		else if (g_inputController->isLeftJustPressed(&m_boundingBoxes[0]))
		{
			onLeftJustPressed();
		}
	}
	
	m_animatedSprite.update(frameTime);
	if (DEBUG_RENDERING && m_isDrawBoundingBox)
	{
		m_debugBox.setPosition(getPosition());
	}
}

void GameObject::setPosition(const sf::Vector2f& position)
{
	// TODO: maybe extend for multiple BBs
	m_position = position;
	m_boundingBoxes[0].left = position.x;
	m_boundingBoxes[0].top = position.y;
	// origin is set to the center for rotation + flipping
	sf::Vector2f spriteCenter(sf::Vector2f(m_boundingBoxes[0].width / 2, m_boundingBoxes[0].height / 2) - m_spriteOffset);
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
	m_animatedSprite.setRotation(angle * DEG_TO_RAD);
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
	// TODO: maybe extend for multiple BBs
	m_boundingBoxes.clear();
	m_boundingBoxes.push_back(rect);
}

const sf::FloatRect* GameObject::getBoundingBox() const
{
	return &m_boundingBoxes.at(0);
}

const std::vector<sf::FloatRect> *GameObject::getBoundingBoxes() const
{
	return &m_boundingBoxes;
}

void GameObject::onMouseOver() 
{
	// nop
}

void GameObject::onInteractKey()
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
	// TODO: maybe extend for multiple BBs
	return sf::Vector2f(
		m_boundingBoxes[0].left + (m_boundingBoxes[0].width / 2),
		m_boundingBoxes[0].top + (m_boundingBoxes[0].height / 2));
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

Screen* GameObject::getScreen() const
{
	return m_screen;
}
