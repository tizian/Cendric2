#include "GameObject.h"

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::load()
{
	//nop
}

void GameObject::setCurrentAnimation(Animation* animation, bool isFlipped)
{
	m_animatedSprite.setFlipped(isFlipped);
	m_animatedSprite.setAnimation(animation);
}

void GameObject::addAnimation(GameObjectState state, Animation& animation)
{
	m_animations.insert({ state, animation });
}
Animation* GameObject::getAnimation(GameObjectState state)
{
	return &m_animations[state];
}

void GameObject::render(sf::RenderTarget &renderTarget) const
{
	renderTarget.draw(m_animatedSprite);
}

void GameObject::update(sf::Time& frameTime)
{
	m_animatedSprite.update(frameTime);
}

void GameObject::setPosition(const sf::Vector2f& position)
{
	Object::setPosition(position);
	m_animatedSprite.setPosition(position + m_spriteOffset);
}

void GameObject::setFrameTime(sf::Time time) 
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

