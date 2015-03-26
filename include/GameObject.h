#include "stdafx.h"

enum class GameObjectState
{
	Idle,
	Walking,
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
	void setPosition(const Vector2f& position) override;
	void setSpriteOffset(sf::Vector2f& spriteOffset);
	void setFrameTime(sf::Time time);
	void playCurrentAnimation(bool play);
	void loopCurrentAnimation(bool loop);
	Animation* getAnimation(GameObjectState state);
	
private:
	map<GameObjectState, Animation> m_animations;
	AnimatedSprite m_animatedSprite;
	sf::Vector2f m_spriteOffset;
};