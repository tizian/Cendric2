#include "Screens/WorldScreen.h"
#include "Screens/LoadingScreen.h"
#include "GUI/BookWindow.h"
#include "Item.h"
#include "Trigger.h"

using namespace std;

const std::string vertexShader = \
"void main()" \
"{" \
"    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;" \
"    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;" \
"    gl_FrontColor = gl_Color;" \
"}";

const std::string lightFragmentShader = \
"uniform sampler2D texture;" \
"uniform float ambientLevel;" \
"" \
"void main()" \
"{" \
"    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);" \
"    float lightLevel = max(0.0, ambientLevel - pixel.r);" \
"    gl_FragColor = vec4(0.0, 0.0, 0.0, lightLevel);" \
"}";

const std::string foregroundFragmentShader = \
"uniform sampler2D texture;" \
"uniform float ambientLevel;" \
"" \
"void main()" \
"{" \
"    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);" \
"	 float a = pixel.a > 0.0 ? ambientLevel : 0.0;"
"	 gl_FragColor = vec4(0.0, 0.0, 0.0, a); " \
"}";

WorldScreen::WorldScreen(CharacterCore* core) : Screen(core) {
	m_renderTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_renderTexture2.create(WINDOW_WIDTH, WINDOW_HEIGHT);

	m_lightLayerShader.setUniform("texture", sf::Shader::CurrentTexture);
	m_lightLayerShader.loadFromMemory(vertexShader, lightFragmentShader);

	m_foregroundLayerShader.setUniform("texture", sf::Shader::CurrentTexture);
	m_foregroundLayerShader.loadFromMemory(vertexShader, foregroundFragmentShader);

	setTooltipPositionTop(true);

	initParty();
}

WorldScreen::~WorldScreen() {
	delete m_interface;
	delete m_progressLog;
	delete m_bookWindow;
	for (auto& overlay : m_overlayQueue) {
		delete overlay;
	}
	m_overlayQueue.clear();
}

void WorldScreen::notifyPermanentItemConsumed(const Item* item) {
	if (item == nullptr) return;
	getCharacterCore()->addPermanentAttributes(item->getAttributes());

	addScreenOverlay(ScreenOverlay::createPermanentItemScreenOverlay(item));
	m_progressLog->addPermanentItemProgress(item);
	notifyItemChange(item->getID(), -1);
	m_interface->reloadCharacterInfo();
}

void WorldScreen::notifyItemChange(const std::string& itemID, int amount) {
	if (amount == 0) return;
	if (amount > 0) {
		g_resourceManager->playSound(m_pickupSound, GlobalResource::SOUND_GUI_PICKUP, true);
	}
	getCharacterCore()->notifyItemChange(itemID, amount);
	m_progressLog->addItemProgress(itemID, amount);
	m_interface->reloadInventory(itemID);
	m_interface->reloadQuestLog();
	m_interface->reloadCharacterInfo();
}

void WorldScreen::notifyItemConversion(const std::string& oldItemID, const std::string& newItemID, int amount) {
	getCharacterCore()->notifyItemChange(oldItemID, -amount);
	getCharacterCore()->notifyItemChange(newItemID, amount);
	m_progressLog->addItemConversionProgress(oldItemID, newItemID);
	m_interface->reloadInventory(oldItemID);
	m_interface->reloadInventory(newItemID);
	m_interface->reloadQuestLog();
	m_interface->reloadCharacterInfo();
}

void WorldScreen::notifyItemEquip(const std::string& itemID, ItemType type) {
	if (type != ItemType::VOID) {
		getCharacterCore()->equipItem(itemID, type);
	}
	else {
		auto bean = g_databaseManager->getItemBean(itemID);
		if (bean.status != BeanStatus::Filled) return;
		getCharacterCore()->equipItem(bean.item_id, bean.item_type);
	}

	m_interface->reloadInventory();
	m_interface->reloadCharacterInfo();
}

void WorldScreen::notifyQuestConditionFulfilled(const std::string& questID, const std::string& condition) {
	getCharacterCore()->setQuestConditionFulfilled(questID, condition);
	m_progressLog->addQuestConditionFullfilled(questID, condition);
	m_interface->reloadQuestLog();
}

void WorldScreen::notifyQuestTargetKilled(const std::string& questID, const std::string& name) {
	getCharacterCore()->setQuestTargetKilled(questID, name);
	m_progressLog->addQuestTargetKilled(questID, name);
	m_interface->reloadQuestLog();
}

void WorldScreen::notifyQuestStateChanged(const std::string& questID, QuestState state) {
	if (getCharacterCore()->setQuestState(questID, state)) {
		m_progressLog->addQuestStateChanged(questID, state);
		addScreenOverlay(ScreenOverlay::createQuestScreenOverlay(questID, state));
		m_interface->reloadQuestLog();
	}
}
	
void WorldScreen::notifySpellLearned(SpellID id) {
	getCharacterCore()->learnSpell(id);
	addScreenOverlay(ScreenOverlay::createSpellLearnedScreenOverlay(id));
	m_interface->reloadSpellBook();
}

void WorldScreen::notifyModifierLearned(const SpellModifier& modifier) {
	getCharacterCore()->learnModifier(modifier);
	addScreenOverlay(ScreenOverlay::createModifierLearnedScreenOverlay(modifier));
	m_interface->reloadSpellBook();
}

void WorldScreen::notifyQuestDescriptionAdded(const std::string& questID, int descriptionID) {
	if (getCharacterCore()->unlockQuestDescription(questID, descriptionID)) {
		m_progressLog->addQuestDescriptionAdded(questID);
		m_interface->reloadQuestLog();
	}
}

void WorldScreen::notifyConditionAdded(const Condition& condition) {
	if (getCharacterCore()->setConditionFulfilled(condition.type, condition.name)) {
		reloadTriggers();
	}
}

void WorldScreen::notifyReputationAdded(FractionID fraction, int amount) {
	if (amount < 0) return;
	getCharacterCore()->addReputation(fraction, amount);
	m_progressLog->addReputationAdded(fraction, amount);
	m_interface->reloadCharacterInfo();
}

void WorldScreen::notifyHintAdded(const std::string& hintKey) {
	addScreenOverlay(ScreenOverlay::createHintScreenOverlay(hintKey));
	getCharacterCore()->learnHint(hintKey);
	m_interface->reloadCharacterInfo();
}

void WorldScreen::reloadTriggers() {
	for (GameObject* go : *getObjects(GameObjectType::_Overlay)) {
		Trigger* trigger = dynamic_cast<Trigger*>(go);
		if (trigger == nullptr) continue;
		reloadTrigger(trigger);
	}
}

void WorldScreen::toggleGodmode() {
	if (!g_resourceManager->getConfiguration().isGodmode) {
		g_resourceManager->getConfiguration().isGodmode = true;
		setTooltipText("GodmodeActivated", sf::Color::Magenta, true);
	}
	else {
		g_resourceManager->getConfiguration().isGodmode = false;
		setTooltipText("GodmodeDeactivated", sf::Color::Magenta, true);
	}
}

void WorldScreen::reloadTrigger(Trigger* trigger) const {
	if (trigger->getData().condition.empty()) {
		trigger->getData().isTriggerable = true;
		return;
	}

	trigger->getData().isTriggerable = getCharacterCore()->isConditionFulfilled("trigger", trigger->getData().condition);
}

void WorldScreen::initParty() {
	sf::Vector2f pos(10.f, 50.f);
	float posOffset = GUIConstants::CHARACTER_SIZE_L + 10;

	m_partyName.setCharacterSize(GUIConstants::CHARACTER_SIZE_L);
	m_partyName.setColor(COLOR_BAD);
	m_partyName.setTextStyle(TextStyle::Shadowed);
	m_partyName.setPosition(pos);
	if (m_characterCore->getPartyHandler().getData().isPartyActive) {
		m_partyName.setString(m_characterCore->getPartyHandler().getData().name);
	}

	pos.y += posOffset;

	m_partyTime.setCharacterSize(GUIConstants::CHARACTER_SIZE_L);
	m_partyTime.setColor(COLOR_BAD);
	m_partyTime.setTextStyle(TextStyle::Shadowed);
	m_partyTime.setPosition(pos);
	pos.y += posOffset;

	m_partyScore.setCharacterSize(GUIConstants::CHARACTER_SIZE_L);
	m_partyScore.setColor(COLOR_BAD);
	m_partyScore.setTextStyle(TextStyle::Shadowed);
	m_partyScore.setPosition(pos);
}

void WorldScreen::updateParty(const sf::Time& frameTime) {
	if (!m_characterCore->getPartyHandler().getData().isPartyActive) {
		if (!m_partyLockOverlay) {
			m_partyLockOverlay = ScreenOverlay::createPartyLockedScreenOverlay();
		}
		if (!m_partyForm) {
			float width = 450;
			float height = 230;
			m_partyForm = new PartyForm(sf::FloatRect(0.5f * (WINDOW_WIDTH - width), 0.5f * (WINDOW_HEIGHT - height), width, height));
			m_partyForm->setOnOkClicked(std::bind(&WorldScreen::onIDEntered, this));
			addObject(m_partyForm);
			setAllButtonsEnabled(false);
		}

		if (m_partyLockOverlay) {
			m_partyLockOverlay->update(frameTime);
		}
		if (m_partyForm) {
			m_partyForm->update(frameTime);
		}
		return;
	}

	auto& timeLeft = m_characterCore->getPartyHandler().getData().timeLeft;
	updateTime(timeLeft, frameTime);

	// format string
	int seconds = static_cast<int>(std::round(timeLeft.asSeconds()));
	int minutes = seconds / 60;
	seconds = seconds % 60;
	m_partyTime.setString(std::to_string(minutes) + ":" + std::to_string(seconds));
	m_partyScore.setString(std::to_string(m_characterCore->getPartyHandler().getData().score));

	if (timeLeft == sf::Time::Zero) {
		m_characterCore->getPartyHandler().endCurrentSession();
	}
}

void WorldScreen::updateOverlayQueue() {
	if (m_overlayQueue.empty()) return;
	if (!getObjects(GameObjectType::_ScreenOverlay)->empty()) return;
	addObject(m_overlayQueue.at(0));
	m_overlayQueue.erase(m_overlayQueue.begin());
}

Inventory* WorldScreen::getInventory() {
	if (m_interface == nullptr) return nullptr;
	return m_interface->getInventory();
} 

ProgressLog* WorldScreen::getProgressLog() {
	return m_progressLog;
}

void WorldScreen::loadWeather() {
	delete m_weatherSystem;
	m_weatherSystem = new WeatherSystem();
	const WorldData* worldData = getWorldData();
	const WeatherData* weatherData = m_characterCore->getWeather(worldData->id);
	bool isLevel = dynamic_cast<const LevelData*>(worldData) != nullptr;
	if (weatherData) {
		m_weatherSystem->load(weatherData, isLevel);
	}
	else {
		m_weatherSystem->load(&worldData->weather, isLevel);
	}
}

void WorldScreen::execUpdate(const sf::Time& frameTime) {
	updateOverlayQueue();
	
	m_interface->update(frameTime);
	m_progressLog->update(frameTime);

	if (g_inputController->isKeyJustPressed(Key::Screenshot)) {
		char buff[20];
		std::time_t t = time(nullptr);
		strftime(buff, 20, "%Y-%m-%d %H-%M-%S", localtime(&t));

		std::string file = getDocumentsPath(GlobalResource::SCREENSHOT_FOLDER) + string(buff) + ".png";

		sf::Image image = g_renderWindow->capture();
		image.saveToFile(file);

		std::string logString = "Saved screenshot under \"" + file + "\".";
		g_logger->logInfo("WorldScreen", logString);

		setTooltipText("ScreenshotSaved", COLOR_GOOD, true);
	}

	if (g_inputController->isKeyJustPressed(Key::Quickload)) {
		quickload();
	}
	else if (g_inputController->isKeyJustPressed(Key::Quicksave)) {
		quicksave();
	}

	if (g_inputController->isKeyJustPressed(Key::Debug)) {
		ConfigurationData& config = g_resourceManager->getConfiguration();
		config.isDebugRendering = !config.isDebugRendering;
	}
}

void WorldScreen::addScreenOverlay(ScreenOverlay* overlay) {
	if (overlay == nullptr) return;
	m_overlayQueue.push_back(overlay);
}

void WorldScreen::quickload() {
	CharacterCore* newCharacterCore = new CharacterCore();
	if (!newCharacterCore->quickload()) {
		// no quicksave exists
		setTooltipText("NoQuicksaveExists", COLOR_BAD, true);
		delete newCharacterCore;
	}
	else {
		delete m_characterCore;
		m_characterCore = nullptr;
		setNextScreen(new LoadingScreen(newCharacterCore));
		return;
	}
}

void WorldScreen::onIDEntered() {
	int newID = std::atoi(m_partyForm->getEnteredString().c_str());
	m_partyForm = nullptr;
	std::string userName = m_characterCore->getPartyHandler().checkID(newID);
	if (userName.empty()) {
		return;
	}

	m_partyLockOverlay->setPermanent(false);
	m_partyLockOverlay = nullptr;

	m_characterCore->getPartyHandler().startNewSession(newID, userName);

	m_partyName.setString(userName);
}

void WorldScreen::quicksave() {
	m_characterCore->quicksave();
	setTooltipText("GameSaved", COLOR_GOOD, true);
}

void WorldScreen::execOnExit(const Screen* nextScreen) {
	for (auto& it : m_overlayQueue) {
		delete it;
	}
	m_overlayQueue.clear();
}

void WorldScreen::setBook(const Item& document) {
	m_interface->hideAll();

	delete m_bookWindow;
	m_bookWindow = new BookWindow(document);
	m_bookWindowDisposed = false;
	m_bookWindow->addCloseButton([&]() {
		m_bookWindowDisposed = true;
	});
}

void WorldScreen::handleBookWindow(const sf::Time& frameTime) {
	if (m_bookWindow == nullptr) return;
	if (!m_bookWindow->updateWindow(frameTime) || m_bookWindowDisposed) {
		delete m_bookWindow;
		m_bookWindow = nullptr;
	}
	updateProgressLog(frameTime);
	updateTooltipText(frameTime);
	updateObjects(GameObjectType::_Light, frameTime);
}

void WorldScreen::updateProgressLog(const sf::Time& frameTime) {
	m_progressLog->update(frameTime);
}

void WorldScreen::render(sf::RenderTarget& renderTarget) {
	m_progressLog->render(renderTarget);
	m_interface->render(renderTarget);

	renderTarget.draw(m_partyName);
	renderTarget.draw(m_partyScore);
	renderTarget.draw(m_partyTime);
}

void WorldScreen::renderAfterForeground(sf::RenderTarget& renderTarget) {
	m_interface->renderAfterForeground(renderTarget);

	if (m_bookWindow != nullptr) {
		m_bookWindow->render(renderTarget);
	}
}
