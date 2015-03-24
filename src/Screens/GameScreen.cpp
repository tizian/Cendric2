#include "stdafx.h"

GameScreen::GameScreen()
{
	cout << "Init GameScreen...";
	m_currentLevel.loadFromFile(g_resourceManager->getFilename(ResourceID::Level_testLevel));
	MainCharacter* mainChar = new MainCharacter();
	mainChar->load();
	addObject(mainChar);
	cout << "...done." << endl;
}

Screen* GameScreen::update(sf::Time frameTime)
{
	return Screen::update(frameTime);
}

void GameScreen::render(sf::RenderTarget &renderTarget) const
{
	m_currentLevel.draw(renderTarget, RenderStates::Default);
	Screen::render(renderTarget);
}