#include "stdafx.h"

GameScreen::GameScreen()
{
	cout << "Init GameScreen...";
	m_currentLevel.loadFromFile("res/level/testlevel.dric");
	cout << "...done." << endl;
}

void GameScreen::update(float dt)
{

}

void GameScreen::render(sf::RenderTarget &renderTarget) const
{
	m_currentLevel.draw(renderTarget, RenderStates::Default);
}