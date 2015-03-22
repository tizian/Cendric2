#include "stdafx.h"

class GameScreen : public Screen
{
public:
	GameScreen();

	Screen* update(sf::Time frameTime) override;
	void render(sf::RenderTarget &renderTarget) const override;

private:
	Level m_currentLevel;
};