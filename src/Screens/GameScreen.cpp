#include "Screens\GameScreen.h"

using namespace std;

GameScreen::GameScreen(ResourceID levelID)
{
	m_currentLevel.load(levelID);
	m_mainChar = new MainCharacter(&m_currentLevel);
	m_mainChar->setScreen(this);
	addObject(m_mainChar);
}

void GameScreen::onEnter(Screen *previousScreen)
{
}

void GameScreen::onExit(Screen *nextScreen)
{
	m_currentLevel.dispose();
	delete m_mainChar;
}

Screen* GameScreen::update(sf::Time frameTime)
{
	return Screen::update(frameTime);
}

void GameScreen::render(sf::RenderTarget &renderTarget)
{
	// parallax, maybe forground + background layers?
	m_currentLevel.draw(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	Screen::render(renderTarget);
}