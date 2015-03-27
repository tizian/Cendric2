#include "stdafx.h"

const float WALK_ACCELERATION = 2000.0f;
const float JUMP_BOOST = -15000.0f;

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
	// handle input and calculate the next position
	void handleInput();
	// update animation based on the current velocity + grounded
	void updateAnimation();
	bool m_isFacingRight;
	bool m_nextIsFacingRight;
	GameObjectState m_state;
	bool m_isGrounded;
	sf::Vector2f m_nextPosition;
	Level* m_level;
};