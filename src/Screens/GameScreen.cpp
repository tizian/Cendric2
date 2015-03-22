#include "stdafx.h"

GameScreen::GameScreen()
{
	cout << "Init GameScreen...";
	m_currentLevel.loadFromFile("res/level/testlevel.dric");
	cout << "...done." << endl;
}

Screen* GameScreen::update(float dt)
{
	return this;
}

void GameScreen::render(sf::RenderTarget &renderTarget) const
{
	m_currentLevel.draw(renderTarget, RenderStates::Default);
}