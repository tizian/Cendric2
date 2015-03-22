#include "stdafx.h"

class GameScreen : public Screen
{
public:
	GameScreen();

	void update(float dt) override;
	void render(sf::RenderTarget &renderTarget) const override;

private:
	Level m_currentLevel;
};