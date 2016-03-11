#pragma once

#include <map>

#include "global.h"
#include "AnimatedSprite.h"
#include "GameObject.h"

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
	void setCurrentAnimation(const Animation* animation, bool isFlipped);
	void setSpriteOffset(const sf::Vector2f& spriteOffset);
	// angle is in radian
	virtual void setSpriteRotation(float angle);
	void setState(GameObjectState state) override;
	
	const Animation* getAnimation(GameObjectState state);
	const sf::Vector2f& getSpriteOffset() const;
	const sf::Color& getCurrentSpriteColor() const;

	virtual void setSpriteColor(const sf::Color& color, const sf::Time& time);

protected:
	AnimatedSprite m_animatedSprite;
	std::map<GameObjectState, Animation*> m_animations;
	sf::Vector2f m_spriteOffset = sf::Vector2f(0.f, 0.f);

private:
	// the sprite will reset its color as soon as this time is zero.
	sf::Time m_coloredTime = sf::Time::Zero;
	sf::Color m_currentSpriteColor = COLOR_WHITE;
};

