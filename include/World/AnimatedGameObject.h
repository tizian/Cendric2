#pragma once

#include "global.h"
#include "World/AnimatedSprite.h"
#include "World/GameObject.h"

/* A game object with sprites an animations */
class AnimatedGameObject : public virtual GameObject {
public:
	AnimatedGameObject() : GameObject() {}
	virtual ~AnimatedGameObject();

	virtual void update(const sf::Time& frameTime) override;
	virtual void render(sf::RenderTarget& renderTarget) override;

	/*
	insert an animation for a GameObjectState into the animation map.
	if that state already has an animation, it will delete the old animation and replace it with this animation.
	*/
	void addAnimation(GameObjectState state, Animation* animation);
	void playCurrentAnimation(bool play);
	void loopCurrentAnimation(bool loop);

	virtual void setPosition(const sf::Vector2f& pos) override;
	// if force is set to true, the animation gets locked until "unlockAnimation" is called.
	void setCurrentAnimation(const Animation* animation, bool isFlippedX = false, bool force = false);
	void setSpriteOffset(const sf::Vector2f& spriteOffset);
	void unlockAnimation();
	// angle is in radian
	virtual void setSpriteRotation(float angle);
	virtual void setState(GameObjectState state, bool updateAnimation);
	virtual void setState(GameObjectState state) override;
	
	const Animation* getAnimation(GameObjectState state) const;
	const sf::Vector2f& getSpriteOffset() const;
	const sf::Color& getCurrentSpriteColor() const;
	bool isAnimationLocked() const;

	virtual void setSpriteColor(const sf::Color& color, const sf::Time& time);

protected:
	AnimatedSprite m_animatedSprite;
	std::map<GameObjectState, Animation*> m_animations;
	sf::Vector2f m_spriteOffset = sf::Vector2f(0.f, 0.f);
	bool m_isAnimationLocked = false;

private:
	// the sprite will reset its color as soon as this time is zero.
	sf::Time m_coloredTime = sf::Time::Zero;
	sf::Color m_currentSpriteColor = COLOR_WHITE;
};

