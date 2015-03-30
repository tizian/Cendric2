#include "stdafx.h"

GameScreen::GameScreen()
{
	cout << "Init GameScreen...";
	m_currentLevel.load(ResourceID::Level_testLevel);
	m_mainChar = new MainCharacter(&m_currentLevel);
	addObject(m_mainChar);
	cout << "...done." << endl;
}

GameScreen::~GameScreen()
{
	m_currentLevel.dispose();
}

Screen* GameScreen::update(sf::Time frameTime)
{
	return Screen::update(frameTime);
}

void GameScreen::render(sf::RenderTarget &renderTarget)
{
	// parallax 
	m_currentLevel.draw(renderTarget, RenderStates::Default, m_mainChar->getCenter());
	Screen::render(renderTarget);
}