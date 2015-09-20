#include "Screens/GameScreen.h"

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
	"    if (pixel.a == 1.0)" \
	"		gl_FragColor = vec4(0.0, 0.0, 0.0, ambientLevel); " \
	"    else" \
	"        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);" \
	"}";

GameScreen::GameScreen(CharacterCore* core) : Screen(core)
{
	m_renderTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);

	m_lightLayerShader.setParameter("texture", sf::Shader::CurrentTexture);
	m_lightLayerShader.loadFromMemory(vertexShader, lightFragmentShader);

	m_foregroundLayerShader.setParameter("texture", sf::Shader::CurrentTexture);
	m_foregroundLayerShader.loadFromMemory(vertexShader, foregroundFragmentShader);

	setTooltipPositionTop(true);
}

GameScreen::~GameScreen()
{
	delete m_interface;
	delete m_progressLog;
}

void GameScreen::notifyItemChange(const std::string& itemID, int amount)
{
	getCharacterCore()->notifyItemChange(itemID, amount);
	m_progressLog->addItemProgress(itemID, amount);
	m_interface->reloadInventory(itemID);
	m_interface->reloadQuestLog();
}

void GameScreen::notifyQuestConditionFulfilled(const std::string& questID, const std::string& condition)
{
	getCharacterCore()->setQuestConditionFulfilled(questID, condition);
	m_progressLog->addQuestConditionFullfilled(questID, condition);
	m_interface->reloadQuestLog();
}

void GameScreen::notifyQuestTargetKilled(const std::string& questID, const std::string& name)
{
	getCharacterCore()->setQuestTargetKilled(questID, name);
	m_progressLog->addQuestTargetKilled(questID, name);
	m_interface->reloadQuestLog();
}

void GameScreen::notifyQuestStateChanged(const std::string& questID, QuestState state)
{
	getCharacterCore()->setQuestState(questID, state);
	m_progressLog->addQuestStateChanged(questID, state);
	m_interface->reloadQuestLog();
}

Inventory* GameScreen::getInventory()
{
	if (m_interface == nullptr) return nullptr;
	return m_interface->getInventory();
}

Screen* GameScreen::update(const sf::Time& frameTime)
{
	m_interface->update(frameTime);
	m_progressLog->update(frameTime);
	return this;
}

void GameScreen::updateProgressLog(const sf::Time& frameTime)
{
	m_progressLog->update(frameTime);
}

void GameScreen::render(sf::RenderTarget &renderTarget)
{
	m_interface->render(renderTarget);
	m_progressLog->render(renderTarget);
}
