#pragma once

#include <map>

#include "global.h"
#include "Object.h"
#include "AnimatedSprite.h"

enum class GameObjectState
{
	Idle,
	Idle_up,
	Idle_down,
	Idle_left,
	Idle_right,
	Walking,
	Walking_up,
	Walking_down,
	Walking_right,
	Walking_left,
	Jumping
};

// A game object with animations
class GameObject : public Object
{
public:
	GameObject();
	~GameObject();

	// \brief loads animations using the resource manager
	// also defines bounding box and sprite offset
	virtual void load();

	void render(sf::RenderTarget& renderTarget) const override;
	void update(sf::Time& frameTime) override;

	void setCurrentAnimation(Animation* animation, bool isFlipped);
	void addAnimation(GameObjectState state, Animation& animation);
	void setPosition(const sf::Vector2f& position) override;
	void setSpriteOffset(const sf::Vector2f& spriteOffset);
	void setFrameTime(sf::Time time);
	void playCurrentAnimation(bool play);
	void loopCurrentAnimation(bool loop);
	Animation* getAnimation(GameObjectState state);
	
private:
	std::map<GameObjectState, Animation> m_animations;
	AnimatedSprite m_animatedSprite;
	sf::Vector2f m_spriteOffset;
};