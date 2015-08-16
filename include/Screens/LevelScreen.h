#pragma once

#include "global.h"
#include "Level.h"
#include "LevelMainCharacter.h"
#include "Screen.h"
#include "ResourceManager.h"
#include "LevelMainCharacterLoader.h"
#include "LevelInterface.h"

#include "GUI/Button.h"
#include "Enums/LevelID.h"

class LevelScreen : public Screen
{
public:
	LevelScreen(LevelID levelID, CharacterCore* core);

	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;
	void execOnEnter(const Screen* previousScreen) override;
	void execOnExit(const Screen* nextScreen) override;

	void addBuffToInterface(BuffType type, const sf::IntRect& textureLocation, const sf::Time& duration) const;

private:
	Level m_currentLevel;
	LevelMainCharacter* m_mainChar;
	LevelID m_levelID;
	LevelInterface* m_interface;	

	bool m_isGameOver = false;
	BitmapText* m_youDied = nullptr;
	sf::Sprite* m_gameOverSprite = nullptr;
	Button* m_retryButton = nullptr;
	Button* m_backToMenuButton = nullptr;
};