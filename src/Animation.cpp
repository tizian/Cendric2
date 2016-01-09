#include "Animation.h"

void Animation::addFrame(const sf::IntRect& rect) {
	m_frames.push_back(rect);
}

void Animation::clearFrames() {
	m_frames.clear();
}

void Animation::setSpriteSheet(const sf::Texture* texture) {
	m_texture = texture;
}

void Animation::setFrameTime(const sf::Time& frameTime) {
	m_frameTime = frameTime;
}

const sf::Texture* Animation::getSpriteSheet() const {
	return m_texture;
}

size_t Animation::getSize() const {
	return m_frames.size();
}

const sf::Time& Animation::getFrameTime() const {
	return m_frameTime;
}

const sf::IntRect& Animation::getFrame(size_t n) const {
	return m_frames[n];
}