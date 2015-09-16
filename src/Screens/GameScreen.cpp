#include "Screens/GameScreen.h"

using namespace std;

GameScreen::GameScreen(CharacterCore* core) : Screen(core)
{
}

GameScreen::~GameScreen()
{
	delete m_interface;
}

void GameScreen::notifyItemChange(const std::string& itemID, int amount)
{
	// add to core
	m_characterCore->notifyItemChange(itemID, amount);
	m_progressLog.addItemProgress(itemID, amount);
	m_interface->reloadInventory(itemID);
}

void GameScreen::notifyQuestConditionFulfilled(const std::string& questID, const std::string& condition)
{
	m_characterCore->setQuestConditionFulfilled(questID, condition);
	m_progressLog.addQuestConditionFullfilled(questID, condition);
	m_interface->reloadQuestLog(questID);
}

void GameScreen::notifyQuestTargetKilled(const std::string& questID, const std::string& name)
{
	m_characterCore->setQuestTargetKilled(std::make_pair(questID, name));
	m_progressLog.addQuestTargetKilled(questID, name);
	m_interface->reloadQuestLog(questID);
}

void GameScreen::notifyQuestStateChanged(const std::string& questID, QuestState state)
{
	m_characterCore->setQuestState(questID, state);
	m_progressLog.addQuestStateChanged(questID, state);
	m_interface->reloadQuestLog(questID);
}

Screen* GameScreen::update(const sf::Time& frameTime)
{
	m_interface->update(frameTime);
	m_progressLog.update(frameTime);
	return this;
}

void GameScreen::render(sf::RenderTarget &renderTarget)
{
	m_interface->render(renderTarget);
	m_progressLog.render(renderTarget);
}
