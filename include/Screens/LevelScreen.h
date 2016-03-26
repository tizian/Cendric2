#pragma once

#include "global.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"
#include "WorldScreen.h"
#include "ResourceManager.h"
#include "Level/LevelMainCharacterLoader.h"
#include "Level/LevelInterface.h"

#include "GUI/Button.h"
#include "GUI/YesOrNoForm.h"
#include "GUI/ProgressLog.h"

class LevelScreen : public WorldScreen {
public:
	LevelScreen(const std::string& levelID, CharacterCore* core);
	// called by the loading screen
	void load();

	void execUpdate(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;
	void execOnEnter(const Screen* previousScreen) override;
	void execOnExit(const Screen* nextScreen) override;

	void addSpellBuffToInterface(const sf::IntRect& textureLocation, const sf::Time& duration, Spell* spell, const AttributeData& attr);
	void addFoodBuffToInterface(const sf::IntRect& textureLocation, const sf::Time& duration, const std::string& itemID, const AttributeData& attr);
	void addDotBuffToInterface(const sf::IntRect& textureLocation, const sf::Time& duration, const DamageOverTimeData& data);
	void removeTypedBuffs(SpellID id);

	// called by the loading screen. the dynamic tiles & light in level
	void loadForRenderTexture();

	// the level screen doesn't return the original core here, but a mere copy.
	CharacterCore* getCharacterCore() const override;
	// the level screen does not use the original core, but runs on a copy.
	// to update the original core, call this method. (used by the checkpoints and when leaving the world)
	void writeToCore();
	bool exitWorld() override;

	LevelMainCharacter* getMainCharacter() const override;
	const Level* getWorld() const override;

private:
	Level m_currentLevel;
	sf::Music m_backgroundMusic;
	LevelMainCharacter* m_mainChar = nullptr;
	std::string m_levelID;

	bool m_isGameOver = false;
	BitmapText* m_overlayText = nullptr;
	sf::Sprite* m_overlaySprite = nullptr;
	YesOrNoForm* m_yesOrNoForm = nullptr;
	Button* m_retryButton = nullptr;
	Button* m_backToMenuButton = nullptr;
	Button* m_resumeButton = nullptr;

	void handleGameOver(const sf::Time& frameTime);

	// pretty little agents to give to our yes or no form and buttons
	void onNo();
	void onYesToCheckpoint();
	void onBackToCheckpoint();
	void onBackToMenu();
	void onRetry();
	void onResume();

	// the level screen runs on a copy of the character core that only gets written back to the original core
	// if a checkpoint is reached or the level is finished.
	CharacterCore* m_characterCoreCopy = nullptr;

	void cleanUp();
};