#include "stdafx.h"

// TODO maybe move to movable game object.
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
	virtual void load();

	void render(sf::RenderTarget &renderTarget) const override;
	void update(sf::Time frameTime) override;

	void setCurrentAnimation(Animation* animation, bool isFlipped);
	void addAnimation(GameObjectState state, Animation& animation);
	void setPosition(Vector2f& position);
	void setFrameTime(sf::Time time);
	void playCurrentAnimation(bool play);
	void loopCurrentAnimation(bool loop);
	Animation* getAnimation(GameObjectState state);
	const Vector2f& getPosition();
	
private:
	map<GameObjectState, Animation> m_animations;
	AnimatedSprite m_animatedSprite;
};