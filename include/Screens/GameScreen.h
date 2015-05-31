#pragma once

#include "global.h"
#include "Level.h"
#include "LevelMainCharacter.h"
#include "Screen.h"
#include "ResourceManager.h"
#include "LevelMainCharacterLoader.h"
#include "LevelInterface.h"

#include "Enums/LevelID.h"

class GameScreen : public Screen
{
public:
	GameScreen(LevelID levelID, CharacterCore* core);

	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;
	void execOnEnter(const Screen* previousScreen) override;
	void execOnExit(const Screen* nextScreen) override;

private:
	Level m_currentLevel;
	LevelMainCharacter* m_mainChar;
	LevelID m_levelID;
	LevelInterface m_interface;	
};