#include "stdafx.h"

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

void GameObject::setCurrentAnimation(Animation& animation)
{
	m_animatedSprite.setAnimation(animation);
	m_animatedSprite.play();
}

void GameObject::addAnimation(std::string tag, Animation& animation)
{
	m_animations.insert({ tag, animation });
}
Animation GameObject::getAnimation(std::string tag)
{
	return m_animations.at(tag);
}

void GameObject::render(sf::RenderTarget &renderTarget) const
{
	renderTarget.draw(m_animatedSprite);
}

void GameObject::update(sf::Time frameTime)
{
	m_animatedSprite.update(frameTime);
}

