#include "stdafx.h"

BackgroundLayer::BackgroundLayer()
{
}

BackgroundLayer::~BackgroundLayer()
{
}

void BackgroundLayer::dispose()
{
	g_resourceManager->deleteResource(m_fileName);
}

void BackgroundLayer::load(std::string& filename, float distance)
{
	m_fileName = filename;
	m_distance = distance;
	m_sprite = sf::Sprite((*g_resourceManager->getTexture(m_fileName)));
}

void BackgroundLayer::render(sf::RenderTarget &target, sf::RenderStates states)
{
	target.draw(m_sprite, states);
}

float BackgroundLayer::getDistance()
{
	return m_distance;
}