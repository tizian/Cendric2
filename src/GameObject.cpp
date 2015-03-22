#include "stdafx.h"

GameObject::GameObject()
	: m_isLoaded(false)
{
}


GameObject::~GameObject()
{
}

void GameObject::load(std::string filename)
{
	if (!m_texture.loadFromFile(filename))
	{
		m_filename = "";
		m_isLoaded = false;
	}
	else
	{
		m_filename = filename;
		m_sprite.setTexture(m_texture);
		m_isLoaded = true;
	}
}

void GameObject::draw(sf::RenderWindow & window)
{
	if (m_isLoaded)
	{
		window.draw(m_sprite);
	}
}

void GameObject::setPosition(float x, float y)
{
	if (m_isLoaded)
	{
		m_sprite.setPosition(x, y);
	}
}