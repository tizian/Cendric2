#pragma once

#include "global.h"
#include "Level.h"
#include "MainCharacter.h"
#include "Screen.h"
#include "ResourceManager.h"
#include "Weapons\IceStaff.h"

class GameScreen : public Screen
{
public:
	GameScreen(ResourceID levelID);

	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;
	void execOnEnter(const Screen* previousScreen) override;
	void execOnExit(const Screen* nextScreen) override;

private:
	Level m_currentLevel;
	MainCharacter* m_mainChar;
	ResourceID m_levelID;
};