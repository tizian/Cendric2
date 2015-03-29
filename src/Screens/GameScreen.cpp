#include "stdafx.h"

GameScreen::GameScreen()
{
	cout << "Init GameScreen...";
	m_currentLevel.load(ResourceID::Level_testLevel);
	MainCharacter* mainChar = new MainCharacter(&m_currentLevel);
	addObject(mainChar);
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
	m_currentLevel.draw(renderTarget, RenderStates::Default);
	Screen::render(renderTarget);
}