#include "stdafx.h"

const float MAX_VELOCITY_X = 10000.0f;
const float MAX_VELOCITY_Y = 1000.0f;
const float WALK_ACCELERATION = 5000.0f;
const float JUMP_BOOST = 700.0f;
const float DAMPING = 0.7f;

// Cendric
class MainCharacter : public MovableGameObject
{
public:
	MainCharacter(Level* level);
	~MainCharacter();

	void load() override;
	void update(sf::Time& frameTime) override;
	void checkCollisions(sf::Vector2f nextPosition) override;

private:
	void handleInput();
	bool m_isFacingRight;
	GameObjectState m_state;
	bool m_grounded;
	sf::Vector2f m_nextPosition;
	Level* m_level;
};