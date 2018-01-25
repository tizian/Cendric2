#include "Screens/LevelScreen.h"
#include "Screens/LoadingScreen.h"
#include "Screens/MenuScreen.h"
#include "Screens/ScreenManager.h"
#include "Level/Enemies/ObserverEnemy.h"
#include "ObjectFactory.h"
#include "GUI/BookWindow.h"
#include "Level/LevelMainCharacterLoader.h"
#include "GUI/Stopwatch.h"

LevelScreen::LevelScreen(const std::string& levelID, CharacterCore* core) : Screen(core), WorldScreen(core) {
	m_levelID = levelID;
	m_particleBGRenderTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_particleFGRenderTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_particleEQRenderTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_equipmentRenderTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);

	m_particleBlendMode = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::OneMinusSrcAlpha, sf::BlendMode::Equation::Add,
		sf::BlendMode::SrcAlpha, sf::BlendMode::One, sf::BlendMode::Add);
}

void LevelScreen::loadSync() {
	m_currentLevel.loadForRenderTexture();

	m_interface = new LevelInterface(this, m_mainChar);
	dynamic_cast<LevelInterface*>(m_interface)->setSpellManager(m_mainChar->getSpellManager());
	dynamic_cast<LevelInterface*>(m_interface)->setPermanentCore(m_characterCore);
	updateMonitoredQuestItems();
}

void LevelScreen::loadAsync() {
	if (!(m_currentLevel.load(m_levelID, this))) {
		std::string errormsg = m_levelID + ": file corrupted!";
		g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
		return;
	}

	m_mainChar = LevelMainCharacterLoader::loadMainCharacter(this, &m_currentLevel);
	m_currentLevel.loadAfterMainChar(m_mainChar);
	LevelMainCharacterLoader::loadEquipment(this);
	m_progressLog = new ProgressLog(getCharacterCore());
	m_progressLog->setYOffset(150.f);

	// load screen
	m_resumeButton = new Button(sf::FloatRect(0, 0, 50, 50), GUIOrnamentStyle::MEDIUM);
	m_resumeButton->setText("Resume");
	m_resumeButton->setVisible(false);
	m_resumeButton->setOnClick(std::bind(&LevelScreen::onResume, this));
	addObject(m_resumeButton);

	m_retryButton = new Button(sf::FloatRect(0, 0, 50, 50), GUIOrnamentStyle::MEDIUM);
	m_retryButton->setText("BackToCheckpoint");
	m_retryButton->setVisible(false);
	m_retryButton->setOnClick(std::bind(&LevelScreen::onBackToCheckpoint, this));
	addObject(m_retryButton);

	m_backToMapButton = new Button(sf::FloatRect(0, 0, 50, 50), GUIOrnamentStyle::MEDIUM);
	m_backToMapButton->setText("BackToMap");
	m_backToMapButton->setVisible(false);
	m_backToMapButton->setOnClick(std::bind(&LevelScreen::onBackToMap, this));
	addObject(m_backToMapButton);

	m_backToMenuButton = new Button(sf::FloatRect(0, 0, 50, 50), GUIOrnamentStyle::MEDIUM);
	m_backToMenuButton->setText("BackToMenu");
	m_backToMenuButton->setVisible(false);
	m_backToMenuButton->setOnClick(std::bind(&LevelScreen::onBackToMenu, this));
	addObject(m_backToMenuButton);

	// synchronize and center buttons
	sf::Vector2f buttonSize(0.f, 50.f);
	buttonSize.x = std::max(std::max(std::max(m_retryButton->getSize().x, m_resumeButton->getSize().x),
		m_backToMenuButton->getSize().x), m_backToMapButton->getSize().x);
	float offsetLeft = (WINDOW_WIDTH - buttonSize.x) * 0.5f;
	m_resumeButton->setSize(buttonSize);
	m_resumeButton->setPosition(sf::Vector2f(offsetLeft, 350.f));
	m_backToMenuButton->setSize(buttonSize);
	m_backToMenuButton->setPosition(sf::Vector2f(offsetLeft, 410.f));
	m_retryButton->setSize(buttonSize);
	m_retryButton->setPosition(sf::Vector2f(offsetLeft, 470.f));
	m_backToMapButton->setSize(buttonSize);
	m_backToMapButton->setPosition(sf::Vector2f(offsetLeft, 530.f));

	g_resourceManager->playMusic(m_currentLevel.getMusicPath());

	// adjust weather
	loadWeather();

	// if config says so, add the stopwatch
	if (g_resourceManager->getConfiguration().isDisplayStopwatch) {
		m_stopwatch = new Stopwatch();
		addObject(m_stopwatch);
	}
}

void LevelScreen::cleanUp() {
	m_currentLevel.dispose();
}

bool LevelScreen::exitWorld() {
	return !m_isGameOver;
}

void LevelScreen::notifyCharacterInfoReload() {
	m_interface->reloadCharacterInfo();
}

void LevelScreen::notifyBackFromMenu() {
	m_mainChar->setQuickcast(g_resourceManager->getConfiguration().isQuickcast);
	g_resourceManager->playMusic(m_currentLevel.getMusicPath());
}

void LevelScreen::notifyBossKilled(const EnemyReward& reward) {
	m_bossRewards.lootableGold += reward.lootableGold;
	for (auto& it : reward.lootableItems) {
		if (!contains(m_bossRewards.lootableItems, it.first)) {
			m_bossRewards.lootableItems.insert(std::make_pair(it.first, 0));
		}
		m_bossRewards.lootableItems[it.first] += it.second;
	}
	for (auto& it : reward.questConditions) {
		m_bossRewards.questConditions.push_back(it);
	}
	for (auto& it : reward.questTargets) {
		m_bossRewards.questTargets.push_back(it);
	}

	// was this the last boss?
	for (auto* go : *getObjects(_Enemy)) {
		if (Enemy* enemy = dynamic_cast<Enemy*>(go)) {
			if (enemy->isBoss() && !enemy->isDead()) {
				return;
			}
		}
	}

	// this was the last boss!
	for (auto& target : m_bossRewards.questTargets) {
		notifyQuestTargetKilled(target.first, target.second);
	}
	for (auto& condition : m_bossRewards.questConditions) {
		notifyQuestConditionFulfilled(condition.first, condition.second);
	}
	for (auto& item : m_bossRewards.lootableItems) {
		notifyItemChange(item.first, item.second);
	}

	notifyItemChange("gold", m_bossRewards.lootableGold);

	addScreenOverlay(ScreenOverlay::createEnemyDefeatedScreenOverlay(m_bossRewards.lootableItems, m_bossRewards.lootableGold), true);
	m_currentLevel.executeBossEnding(true);

	m_interface->hideAll();
	m_isBossDefeated = true;
	m_mainChar->setInputLock();
	m_mainChar->setInvincible(true);
}

void LevelScreen::notifyItemEquip(const std::string& itemID, ItemType type) {
	WorldScreen::notifyItemEquip(itemID, type);
	dynamic_cast<LevelInterface*>(m_interface)->notifyReloadEquipment();
}

Enemy* LevelScreen::spawnEnemy(EnemyID enemyId, const sf::Vector2f& position, int skinNr) {
	Enemy* enemy = ObjectFactory::Instance()->createEnemy(enemyId, getWorld(), this);
	if (enemy == nullptr) {
		g_logger->logError("LevelScreen", "Enemy could not be spawned, unknown id.");
		return nullptr;
	}
	enemy->load(skinNr);

	enemy->setPosition(position);
	enemy->setObjectID(-1);
	enemy->setUnique(false);
	enemy->setDebugBoundingBox(sf::Color::Magenta);

	addObject(enemy);
	return enemy;
}

void LevelScreen::setEnemyForHealthBar(const Enemy* enemy) {
	dynamic_cast<LevelInterface*>(m_interface)->setEnemyForHealthBar(enemy);
}

void LevelScreen::clearConsumedFood() {
	dynamic_cast<LevelInterface*>(m_interface)->clearConsumedFood();
}

void LevelScreen::notifyPermanentItemConsumed(const Item* item) {
	WorldScreen::notifyPermanentItemConsumed(item);
	dynamic_cast<LevelInterface*>(m_interface)->notifyReloadAttributes();
	m_interface->reloadCharacterInfo();
}

void LevelScreen::quicksave() {
	if (m_isGameOver || m_isBossDefeated) return;
	WorldScreen::quicksave();
}

void LevelScreen::execOnEnter() {
	addObject(ScreenOverlay::createLocationScreenOverlay(m_currentLevel.getName(),
		m_currentLevel.getWorldData()->isBossLevel,
		m_currentLevel.getWorldData()->isObserved));
	g_inputController->getCursor().setCursorSkin(TargetActive);
}

void LevelScreen::execOnExit() {
	WorldScreen::execOnExit();
	cleanUp();
	g_inputController->getCursor().setCursorSkin(Pointer);
}

void LevelScreen::addSpellBuffToInterface(const sf::IntRect& textureLocation, const sf::Time& duration, Spell* spell, const AttributeData& attr) {
	dynamic_cast<LevelInterface*>(m_interface)->getBuffBar().
		addSpellBuff(textureLocation, duration, spell, attr);
}

void LevelScreen::addFoodBuffToInterface(const sf::IntRect& textureLocation, const sf::Time& duration, const std::string& itemID, const AttributeData& attr) {
	dynamic_cast<LevelInterface*>(m_interface)->getBuffBar().
		addFoodBuff(textureLocation, duration, itemID, attr);
}

void LevelScreen::addDotBuffToInterface(const sf::IntRect& textureLocation, const sf::Time& duration, const DamageOverTimeData& data) {
	dynamic_cast<LevelInterface*>(m_interface)->getBuffBar().
		addDotBuff(textureLocation, duration, data);
}

void LevelScreen::removeTypedBuffs(SpellID id) {
	if (id == SpellID::VOID) {
		dynamic_cast<LevelInterface*>(m_interface)->getBuffBar().removeFoodBuff();
	}
	else {
		dynamic_cast<LevelInterface*>(m_interface)->getBuffBar().removeTypedSpellBuffs(id);
	}
}

bool LevelScreen::notifyObservers() {
	if (!m_currentLevel.getWorldData()->isObserved) return false;

	for (auto go : *getObjects(_Enemy)) {
		ObserverEnemy* observer = dynamic_cast<ObserverEnemy*>(go);
		if (!observer) continue;

		if (observer->notifyStealing(m_isFirstTimeStealing)) {
			if (m_isFirstTimeStealing) {
				m_isFirstTimeStealing = false;
			}
			else {
				m_isGameOver = true;
				m_respawnWaitTime = sf::seconds(5.0f);
				addObject(ScreenOverlay::createArrestedScreenOverlay());
				m_interface->hideAll();
				m_characterCore->setCharacterJailed();
			}
			return true;
		}
	}

	return false;
}

void LevelScreen::toggleGodmode() {
	WorldScreen::toggleGodmode();
	m_mainChar->setGodmode(g_resourceManager->getConfiguration().isGodmode);
}

LevelMainCharacter* LevelScreen::getMainCharacter() const {
	return m_mainChar;
}

const Level* LevelScreen::getWorld() const {
	return &m_currentLevel;
}

const LevelData* LevelScreen::getWorldData() const {
	return m_currentLevel.getWorldData();
}

sf::RenderTexture& LevelScreen::getParticleBGRenderTexture() {
	return m_particleBGRenderTexture;
}

sf::RenderTexture& LevelScreen::getParticleEQRenderTexture() {
	return m_particleEQRenderTexture;
}

sf::RenderTexture& LevelScreen::getParticleFGRenderTexture() {
	return m_particleFGRenderTexture;
}

void LevelScreen::execUpdate(const sf::Time& frameTime) {
	m_weatherSystem->update(m_mainChar->getPosition(), frameTime);
	handleGameOver(frameTime);
	handleBossDefeated(frameTime);
	handleBackToCheckpoint();

	updateObjects(_Button, frameTime);
	updateObjects(_Form, frameTime);
	updateTooltipText(frameTime);

	if (!m_isPaused) {
		handleBookWindow(frameTime);
		WorldScreen::execUpdate(frameTime);

		// sort Movable Tiles
		depthSortObjects(_MovableTile, false);
		// update objects first for relative velocity
		updateObjectsFirst(_MovableTile, frameTime);
		updateObjectsFirst(_LevelMainCharacter, frameTime);
		updateObjectsFirst(_Enemy, frameTime);
		updateObjectsFirst(_DynamicTile, frameTime);
		updateObjectsFirst(_Spell, frameTime);
		// and then normally
		if (!m_interface->isGuiOverlayVisible()) {
			updateObjects(_ScreenOverlay, frameTime);
		}
		updateObjects(_MovableTile, frameTime);
		updateObjects(_DynamicTile, frameTime);
		updateObjects(_Enemy, frameTime);
		updateObjects(_LevelMainCharacter, frameTime);
		updateObjects(_Equipment, frameTime);
		updateObjects(_Spell, frameTime);
		updateObjects(_Overlay, frameTime);
		if (!m_isGameOver) updateObjects(_LevelItem, frameTime);

		updateObjects(_Light, frameTime);
		m_currentLevel.update(frameTime);
	}

	if (m_gamePausedOverlay) {
		m_gamePausedOverlay->update(frameTime);
	}

	// handle pause
	if (!m_isGameOver && g_inputController->isKeyJustPressed(Key::Escape)) {
		m_isPaused = !m_isPaused;

		if (m_isPaused) {
			m_gamePausedOverlay = ScreenOverlay::createGamePausedScreenOverlay();
		}
		else {
			m_gamePausedOverlay->setPermanent(false);
		}

		m_resumeButton->setVisible(m_isPaused);
		m_backToMenuButton->setVisible(m_isPaused);
		m_backToMapButton->setVisible(m_isPaused && !m_currentLevel.getWorldData()->isBossLevel);
		m_retryButton->setVisible(m_isPaused && !m_currentLevel.getWorldData()->isBossLevel);
	}
}

void LevelScreen::setEquipmentColor(const sf::Color& color) {
	m_equipmentColor = color;
}

void LevelScreen::flushTexture(sf::RenderTarget& renderTarget, sf::RenderTexture& renderTexture, const sf::View& oldView, const sf::BlendMode& mode = sf::BlendAlpha) {
	sf::RenderStates renderStates = sf::RenderStates::Default;
	renderStates.blendMode = mode;

	renderTexture.display();
	m_sprite.setTexture(renderTexture.getTexture());
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_sprite, renderStates);
	renderTarget.setView(oldView);
	renderTexture.clear(sf::Color(0, 0, 0, 0));
}

void LevelScreen::render(sf::RenderTarget& renderTarget) {
	sf::Vector2f focus = m_mainChar->getCenter();
	
	// Render level background and content to window				(Normal level background rendered)
	m_currentLevel.drawBackgroundLayers(renderTarget, sf::RenderStates::Default, focus);
	m_currentLevel.setWorldView(renderTarget, focus);
	m_currentLevel.drawBackground(renderTarget, sf::RenderStates::Default);
	sf::View oldView = renderTarget.getView();
	renderObjects(_DynamicTile, renderTarget);
	renderObjects(_MovableTile, renderTarget);
	flushTexture(renderTarget, m_particleBGRenderTexture, oldView, m_particleBlendMode);
	renderObjects(_LevelItem, renderTarget);

	// the main character and the equipment are rendered on one texture
	m_equipmentRenderTexture.setView(oldView);
	renderObjects(_LevelMainCharacter, m_equipmentRenderTexture);
	renderObjects(_Equipment, m_equipmentRenderTexture);
	m_sprite.setColor(m_equipmentColor);
	flushTexture(renderTarget, m_equipmentRenderTexture, oldView, sf::BlendAlpha);
	flushTexture(renderTarget, m_particleEQRenderTexture, oldView, m_particleBlendMode);
	m_sprite.setColor(COLOR_WHITE);

	renderObjects(_Enemy, renderTarget);
	renderObjects(_Spell, renderTarget);
	flushTexture(renderTarget, m_particleFGRenderTexture, oldView, m_particleBlendMode);
	m_currentLevel.drawLightedForeground(renderTarget, sf::RenderStates::Default);
	renderObjects(_DynamicTile, renderTarget); // dynamic tiles get rendered twice, this one is for the fluid tiles.
	m_currentLevel.drawForeground(renderTarget, sf::RenderStates::Default);

	// Render light sprites to extra buffer							(Buffer contains light levels as grayscale colors)
	m_renderTexture.clear();
	m_renderTexture.setView(oldView);
	renderObjects(_Light, m_renderTexture);
	m_renderTexture.display();

	// Render extra buffer with light level shader to window		(Dimming level + lights added as transparent layer on top of map)
	m_sprite.setTexture(m_renderTexture.getTexture());
	m_lightLayerShader.setUniform("ambientLevel", m_currentLevel.getWeather().ambientDimming);
	m_lightLayerShader.setUniform("lightDimming", m_currentLevel.getWeather().lightDimming);
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_sprite, &m_lightLayerShader);

	// Render overlays on top of level; no light levels here		(GUI stuff on top of everything)
	renderTarget.setView(oldView);
	renderObjects(_Overlay, renderTarget);
	renderObjectsAfterForeground(_Overlay, renderTarget);
	renderObjectsAfterForeground(_MovableTile, renderTarget);
	renderObjectsAfterForeground(_DynamicTile, renderTarget);
	renderObjectsAfterForeground(_LevelItem, renderTarget);
	renderObjectsAfterForeground(_LevelMainCharacter, renderTarget);
	renderObjectsAfterForeground(_Equipment, renderTarget);
	renderObjectsAfterForeground(_Enemy, renderTarget);
	renderObjectsAfterForeground(_Spell, renderTarget);
	renderObjectsAfterForeground(_Light, renderTarget);

	m_weatherSystem->render(renderTarget);

	renderTarget.setView(renderTarget.getDefaultView());
	if (!m_interface->isGuiOverlayVisible()) {
		renderObjects(_ScreenOverlay, renderTarget);
	}
	if (m_gamePausedOverlay) {
		m_gamePausedOverlay->render(renderTarget);
	}
	renderTooltipText(renderTarget);
	WorldScreen::render(renderTarget);
	renderAfterForeground(renderTarget);

	renderObjects(_Button, renderTarget);
	renderObjects(_Form, renderTarget);

	renderTarget.setView(oldView);
}

void LevelScreen::handleBossDefeated(const sf::Time& frameTime) {
	if (!m_isBossDefeated) return;

	if (m_bossDefeatedWaitTime == sf::Time::Zero) return;
	updateTime(m_bossDefeatedWaitTime, frameTime);
	if (m_bossDefeatedWaitTime == sf::Time::Zero) {
		onBackToCheckpoint();
	}
	return;
}

void LevelScreen::handleBookWindow(const sf::Time& frameTime) {
	if (m_bookWindow == nullptr) return;

	if (!m_bookWindow->updateWindow(frameTime) || m_bookWindowDisposed) {
		delete m_bookWindow;
		m_bookWindow = nullptr;
		m_interface->getInventory()->show();
	}
}

void LevelScreen::handleBackToCheckpoint() {
	if (m_isBossDefeated) return;

	if (g_inputController->isKeyJustPressed(Key::BackToCheckpoint)) {
		onBackToCheckpoint();
	}
}

void LevelScreen::handleGameOver(const sf::Time& frameTime) {
	// handle game over
	if (!m_isGameOver && !m_mainChar->isDead()) return;
	if (m_isGameOver) {
		if (m_respawnWaitTime == sf::Time::Zero) return;
		updateTime(m_respawnWaitTime, frameTime);
		if (m_respawnWaitTime == sf::Time::Zero) {
			dynamic_cast<LevelInterface*>(m_interface)->restoreConsumedFood();
			onBackToCheckpoint();
		}
		return;
	}

	m_isGameOver = true;
	m_characterCore->increaseDeathCount(m_levelID);
	m_currentLevel.executeBossEnding(false);
	addObject(ScreenOverlay::createGameOverScreenOverlay());
	m_interface->hideAll();
}

// yes or no form
void LevelScreen::onNo() {
	m_yesOrNoForm->setDisposed();
	setAllButtonsEnabled(true);
}

void LevelScreen::onYesToCheckpoint() {
	setNextScreen(new LoadingScreen(m_characterCore));
	m_yesOrNoForm->setDisposed();
}

void LevelScreen::onBackToMenu() {
	if (m_isGameOver) {
		setNextScreen(new MenuScreen(m_characterCore));
	}
	else {
		setNextScreen(new MenuScreen(m_characterCore), true);
		onResume();
	}
}

void LevelScreen::onBackToCheckpoint() {
	dynamic_cast<LevelInterface*>(m_interface)->clearConsumedFood();
	setNextScreen(new LoadingScreen(m_characterCore));
}

void LevelScreen::onBackToMap() {
	m_characterCore->setInLevel(false);
	setNextScreen(new LoadingScreen(m_characterCore));
}

void LevelScreen::onResume() {
	m_retryButton->setVisible(false);
	m_backToMenuButton->setVisible(false);
	m_resumeButton->setVisible(false);
	m_backToMapButton->setVisible(false);
	m_isPaused = false;
	m_gamePausedOverlay->setPermanent(false);
}
