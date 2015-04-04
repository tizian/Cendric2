#include "Animation.h"

Animation::Animation() : m_texture(NULL)
{

}

void Animation::addFrame(sf::IntRect rect)
{
	m_frames.push_back(rect);
}

void Animation::setSpriteSheet(sf::Texture* texture)
{
	m_texture = texture;
}

const sf::Texture* Animation::getSpriteSheet() const
{
	return m_texture;
}

size_t Animation::getSize() const
{
	return m_frames.size();
}

const sf::IntRect& Animation::getFrame(size_t n) const
{
	return m_frames[n];
}