#include "Screens/WorldScreen.h"
#include "Item.h"

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
"	 gl_FragColor = vec4(0.0, 0.0, 0.0, min(ambientLevel, pixel.a)); " \
"}";

WorldScreen::WorldScreen(CharacterCore* core) : Screen(core) {
	m_renderTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    m_renderTexture2.create(WINDOW_WIDTH, WINDOW_HEIGHT);

	m_lightLayerShader.setUniform("texture", sf::Shader::CurrentTexture);
	m_lightLayerShader.loadFromMemory(vertexShader, lightFragmentShader);

	m_foregroundLayerShader.setUniform("texture", sf::Shader::CurrentTexture);
	m_foregroundLayerShader.loadFromMemory(vertexShader, foregroundFragmentShader);

	setTooltipPositionTop(true);
}

WorldScreen::~WorldScreen() {
	delete m_interface;
	delete m_progressLog;
}

void WorldScreen::notifyPermanentItemConsumed(const Item& item) {
	getCharacterCore()->addPermanentAttributes(item.getAttributes());
	
	addScreenOverlay(ScreenOverlay::createPermanentItemScreenOverlay(item));
	m_progressLog->addPermanentItemProgress(item);
	notifyItemChange(item.getID(), -1);
	m_interface->reloadCharacterInfo();
}

void WorldScreen::notifyItemChange(const std::string& itemID, int amount) {
	getCharacterCore()->notifyItemChange(itemID, amount);
	m_progressLog->addItemProgress(itemID, amount);
	m_interface->reloadInventory(itemID);
	m_interface->reloadQuestLog();
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
	getCharacterCore()->setQuestState(questID, state);
	m_progressLog->addQuestStateChanged(questID, state);
	addScreenOverlay(ScreenOverlay::createQuestScreenOverlay(questID, state));
	m_interface->reloadQuestLog();
}

void WorldScreen::notifyQuestDescriptionAdded(const std::string& questID, int descriptionID) {
	getCharacterCore()->unlockQuestDescription(questID, descriptionID);
	m_progressLog->addQuestDescriptionAdded(questID);
	m_interface->reloadQuestLog();
}

void WorldScreen::notifyConditionAdded(const std::string& conditionType, const std::string& condition) {
	getCharacterCore()->setConditionFulfilled(conditionType, condition);
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

void WorldScreen::execUpdate(const sf::Time& frameTime) {
	updateOverlayQueue();
	m_interface->update(frameTime);
	m_progressLog->update(frameTime);

	if (g_inputController->isKeyJustPressed(Key::Screenshot)) {
		char buff[20];
		std::time_t t = time(nullptr);
		strftime(buff, 20, "%Y-%m-%d %H-%M-%S", localtime(&t));

		std::string file = "screenshots/" + string(buff) + ".png";

		sf::Image image = g_renderWindow->capture();
		image.saveToFile(file);

		std::string logString = "Saved screenshot under \"" + file + "\".";
		g_logger->logInfo("WorldScreen", logString);

		setTooltipText("ScreenshotSaved", sf::Color::Green, true);
	}
}

void WorldScreen::execOnExit(const Screen* nextScreen) {
	for (auto& it : m_overlayQueue) {
		delete it;
	}
	m_overlayQueue.clear();
}

void WorldScreen::updateProgressLog(const sf::Time& frameTime) {
	m_progressLog->update(frameTime);
}

void WorldScreen::render(sf::RenderTarget &renderTarget) {
	m_interface->render(renderTarget);
	m_progressLog->render(renderTarget);
}
