#include "Screens\GameScreen.h"

using namespace std;

GameScreen::GameScreen(ResourceID levelID)
{
	m_currentLevel.load(levelID);
	m_mainChar = new MainCharacter(&m_currentLevel);
	addObject(m_mainChar);
}

void GameScreen::onEnter(Screen *previousScreen)
{
}

void GameScreen::onExit(Screen *nextScreen)
{
	m_currentLevel.dispose();
	m_spells.clear();
	delete m_mainChar;
}

void GameScreen::addSpell(Spell* spell)
{
	m_spells.push_back(spell);
}

Screen* GameScreen::update(sf::Time frameTime)
{
	// delete disposed spells
	for (std::vector<Spell*>::iterator it = m_spells.begin(); it != m_spells.end(); /* DON'T increment here*/)
	{
		if ((*it)->isDisposed())
		{
			delete (*it);
			it = m_spells.erase(it);
		}
		else
		{
			it++;
		}
	}
	// add spell from mainchar
	Spell* firedSpell = m_mainChar->getFiredSpell();
	if (firedSpell != nullptr)
	{
		addSpell(firedSpell);
	}
	for (Spell *spell : m_spells)
	{
		spell->update(frameTime);
	}
	return Screen::update(frameTime);
}

void GameScreen::render(sf::RenderTarget &renderTarget)
{
	// parallax, maybe forground + background layers?
	m_currentLevel.draw(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	Screen::render(renderTarget);
	for (Spell *spell : m_spells)
	{
		spell->render(renderTarget);
	}
}