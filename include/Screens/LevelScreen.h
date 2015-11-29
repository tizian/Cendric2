#pragma once

#include "global.h"
#include "Level.h"
#include "LevelMainCharacter.h"
#include "GameScreen.h"
#include "ResourceManager.h"
#include "LevelMainCharacterLoader.h"
#include "LevelInterface.h"

#include "GUI/Button.h"
#include "GUI/YesOrNoForm.h"
#include "GUI/ProgressLog.h"

class LevelScreen : public GameScreen {
public:
	LevelScreen(const std::string& levelID, CharacterCore* core);
	// called by the loading screen
	void load();

	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;
	void execOnEnter(const Screen* previousScreen) override;
	void execOnExit(const Screen* nextScreen) override;

	void addBuffToInterface(BuffType type, const sf::IntRect& textureLocation, const sf::Time& duration, SpellID id = SpellID::VOID) const;
	void removeTypedBuffs(SpellID id);

	// called by the loading screen. the dynamic tiles & light in level
	void loadForRenderTexture();

	// the level screen doesn't return the original core here, but a mere copy.
	CharacterCore* getCharacterCore() const override;
	// the level screen does not use the original core, but runs on a copy.
	// to update the original core, call this method. (used by the checkpoints)
	void writeToCore();

private:
	Level m_currentLevel;
	sf::Music m_backgroundMusic;
	LevelMainCharacter* m_mainChar = nullptr;
	std::string m_levelID;

	bool m_isGameOver = false;
	BitmapText* m_overlayText = nullptr;
	sf::Sprite* m_overlaySprite = nullptr;
	Button* m_retryButton = nullptr;
	Button* m_backToMenuButton = nullptr;
	YesOrNoForm* m_yesOrNoForm = nullptr;

	// pretty little agents to give to our yes or no form.
	void onNo();
	void onYesToCheckpoint();
	void onYesToMenu();
	bool m_isGoBackToCheckpoint = false;
	bool m_isGoBackToMenu = false;

	// the level screen runs on a copy of the character core that only gets written back to the original core
	// if a checkpoint is reached or the level is finished.
	CharacterCore* m_characterCoreCopy = nullptr;

	void cleanUp();
};