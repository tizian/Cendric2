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

class LevelScreen final : public WorldScreen {
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

	// called by level items and chests. It returns true if an observer is near enough and could have seen
	// how the main char steals something. It will not be looted then and the observer will warn cendric / jail 
	bool notifyObservers();
	void toggleGodmode() override;

	// called by the loading screen. the dynamic tiles & light in level
	void loadForRenderTexture();

	bool exitWorld() override;
	void notifyBackFromMenu() override;
	void notifyBossKilled(const EnemyReward& reward);
	void setEnemyForHealthBar(const Enemy* enemy);
	void clearConsumedFood();

	LevelMainCharacter* getMainCharacter() const override;
	const Level* getWorld() const override;
	const LevelData* getWorldData() const override;

	// extra render textures for particles using additive blend
	sf::RenderTexture& getParticleFGRenderTexture();
	sf::RenderTexture& getParticleBGRenderTexture();

private:
	void quicksave() override;

private:
	Level m_currentLevel;
	LevelMainCharacter* m_mainChar = nullptr;
	std::string m_levelID;

	bool m_isGameOver = false;
	bool m_isBossDefeated = false;
	bool m_isPaused = false;
	bool m_isFirstTimeStealing = true;
	sf::Time m_bossDefeatedWaitTime = sf::seconds(8.0f);
	sf::Time m_respawnWaitTime = sf::seconds(2.5f);
	YesOrNoForm* m_yesOrNoForm = nullptr;
	Button* m_retryButton = nullptr;
	Button* m_backToMenuButton = nullptr;
	Button* m_backToMapButton = nullptr;
	Button* m_resumeButton = nullptr;

	ScreenOverlay* m_gamePausedOverlay = nullptr;

	sf::RenderTexture m_particleBGRenderTexture;
	sf::RenderTexture m_particleFGRenderTexture;
	void flushParticleTexture(sf::RenderTarget& renderTarget, sf::RenderTexture& renderTexture, const sf::View& oldView);

	void handleBookWindow(const sf::Time& frameTime);
	void handleGameOver(const sf::Time& frameTime);
	void handleBackToCheckpoint();
	void handleBossDefeated(const sf::Time& frameTime);
	EnemyReward m_bossRewards;

	// pretty little agents to give to our yes or no form and buttons
	void onNo();
	void onYesToCheckpoint();
	void onBackToCheckpoint();
	void onBackToMenu();
	void onBackToMap();
	void onResume();

	void cleanUp();
};