#include "AnimatedGameObject.h"
#include "ResourceManager.h"

void AnimatedGameObject::setCurrentAnimation(const Animation *animation, bool isFlipped) {
	m_animatedSprite.setFlippedX(isFlipped);
	m_animatedSprite.setAnimation(animation);
}

void AnimatedGameObject::addAnimation(GameObjectState state, Animation &animation) {
	m_animations.insert({ state, animation });
}

const Animation* AnimatedGameObject::getAnimation(GameObjectState state) {
	return &m_animations[state];
}

void AnimatedGameObject::render(sf::RenderTarget &renderTarget) {
	renderTarget.draw(m_animatedSprite);
}

void AnimatedGameObject::update(const sf::Time& frameTime) {
	m_animatedSprite.update(frameTime);
	if (m_coloredTime > sf::Time::Zero) {
		m_coloredTime -= frameTime;
		if (m_coloredTime <= sf::Time::Zero) {
			m_animatedSprite.setColor(sf::Color::White);
			m_coloredTime = sf::Time::Zero;
		}
	}

	GameObject::update(frameTime);
}

void AnimatedGameObject::setPosition(const sf::Vector2f &position) {
	GameObject::setPosition(position);
	// origin is set to the center for rotation + flipping
	sf::Vector2f spriteCenter(sf::Vector2f(m_boundingBox.width / 2, m_boundingBox.height / 2) - m_spriteOffset);
	m_animatedSprite.setOrigin(spriteCenter);
	m_animatedSprite.setPosition(position + m_spriteOffset + spriteCenter);
}

void AnimatedGameObject::setSpriteRotation(float angle) {
	// SFML uses degrees, not radian, hence the conversion here. 
	m_animatedSprite.setRotation(radToDeg(angle));
}

void AnimatedGameObject::playCurrentAnimation(bool play) {
	if (play) {
		m_animatedSprite.play();
	}
	else {
		m_animatedSprite.pause();
	}
}

void AnimatedGameObject::loopCurrentAnimation(bool loop) {
	m_animatedSprite.setLooped(loop);
}

void AnimatedGameObject::setSpriteOffset(const sf::Vector2f &spriteOffset) {
	m_spriteOffset = spriteOffset;
}

void AnimatedGameObject::setState(GameObjectState state) {
	GameObject::setState(state);
	setCurrentAnimation(getAnimation(m_state), false);
}

const sf::Vector2f& AnimatedGameObject::getSpriteOffset() const {
	return m_spriteOffset;
}

void AnimatedGameObject::setSpriteColor(const sf::Color& color, const sf::Time& time) {
	if (time <= sf::Time::Zero) return;
	m_animatedSprite.setColor(color);
	m_coloredTime = time;
}