#include "Screens\GameScreen.h"

using namespace std;

GameScreen::GameScreen(ResourceID levelID)
{
	cout << "Init GameScreen...";
	m_currentLevel.load(levelID);
	m_mainChar = new MainCharacter(&m_currentLevel);
	m_mainChar->setScreen(this);
	addObject(m_mainChar);
	cout << "...done." << endl;
}

GameScreen::~GameScreen()
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
	// parallax 
	m_currentLevel.draw(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	Screen::render(renderTarget);
}