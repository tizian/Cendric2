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

void Animation::setLooped(bool isLooped) {
	m_isLooped = isLooped;
}

const sf::Texture* Animation::getSpriteSheet() const {
	return m_texture;
}

size_t Animation::getSize() const {
	return m_frames.size();
}

bool Animation::isLooped() const {
	return m_isLooped;
}

const sf::Time& Animation::getFrameTime() const {
	return m_frameTime;
}

const sf::Time Animation::getAnimationTime() const {
	return sf::milliseconds(static_cast<int>(m_frames.size()) * m_frameTime.asMilliseconds());
}

const sf::IntRect& Animation::getFrame(size_t n) const {
	return m_frames[n];
}