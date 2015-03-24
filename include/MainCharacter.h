#include "stdafx.h"

// Cendric
class MainCharacter : public GameObject
{
public:
	MainCharacter();
	~MainCharacter();

	void load() override;
	void update(sf::Time frameTime) override;

private:
	void handleInput();
	bool m_isFacingRight;
	GameObjectState m_state;
};