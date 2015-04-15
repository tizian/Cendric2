#pragma once

#include "global.h"
#include "Level.h"
#include "MainCharacter.h"
#include "Screen.h"
#include "ResourceManager.h"
#include "ErrorScreen.h"

class GameScreen : public Screen
{
public:
	GameScreen(ResourceID levelID);

	Screen* update(sf::Time frameTime) override;
	void render(sf::RenderTarget &renderTarget) override;

	void onEnter(Screen *previousScreen) override;
	void onExit(Screen *nextScreen) override;

	void addSpell(Spell* spell);

private:
	Level m_currentLevel;
	MainCharacter* m_mainChar;
	std::vector<Spell *> m_spells;
	bool m_errorOccurred;
};