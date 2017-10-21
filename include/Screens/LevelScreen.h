#pragma once

#include "global.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"
#include "WorldScreen.h"
#include "Level/LevelInterface.h"

#include "GUI/Button.h"
#include "GUI/YesOrNoForm.h"

class LevelScreen final : public WorldScreen {
public:
	LevelScreen(const std::string& levelID, CharacterCore* core);
	
	void execUpdate(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;
	void execOnEnter(Screen* previousScreen) override;
	void execOnExit(Screen* nextScreen) override;

	void addSpellBuffToInterface(const sf::IntRect& textureLocation, const sf::Time& duration, Spell* spell, const AttributeData& attr);
	void addFoodBuffToInterface(const sf::IntRect& textureLocation, const sf::Time& duration, const std::string& itemID, const AttributeData& attr);
	void addDotBuffToInterface(const sf::IntRect& textureLocation, const sf::Time& duration, const DamageOverTimeData& data);
	void removeTypedBuffs(SpellID id);

	// called by level items and chests. It returns true if an observer is near enough and could have seen
	// how the main char steals something. It will not be looted then and the observer will warn cendric / jail 
	bool notifyObservers();
	void toggleGodmode() override;

	bool exitWorld() override;
	void notifyBackFromMenu() override;
	void notifyBossKilled(const EnemyReward& reward);
	void setEnemyForHealthBar(const Enemy* enemy);
	void clearConsumedFood();
	Enemy* spawnEnemy(EnemyID enemyId, const sf::Vector2f& position, int skinNr = 0);

	LevelMainCharacter* getMainCharacter() const override;
	const Level* getWorld() const override;
	const LevelData* getWorldData() const override;

	// extra render textures for particles using special blend
	sf::RenderTexture& getParticleFGRenderTexture();
	sf::RenderTexture& getParticleBGRenderTexture();
	sf::RenderTexture& getParticleEQRenderTexture();

	void setEquipmentColor(const sf::Color& color);

	// the part of the world that can be loaded safely async.
	void loadAsync() override;
	// the rest, that cannot be loaded async
	void loadSync() override;

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

	sf::Color m_equipmentColor = COLOR_WHITE;
	sf::RenderTexture m_particleBGRenderTexture;
	sf::RenderTexture m_particleFGRenderTexture;
	sf::RenderTexture m_particleEQRenderTexture;
	sf::RenderTexture m_equipmentRenderTexture;
	sf::BlendMode m_particleBlendMode;
	void flushTexture(sf::RenderTarget& renderTarget, sf::RenderTexture& renderTexture, const sf::View& oldView, const sf::BlendMode& mode);

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