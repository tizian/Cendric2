#include "GUI/ProgressLog.h"
#include "CharacterCore.h"

ProgressLog::ProgressLog(const CharacterCore* core) : m_core(core) {
}

ProgressLog::~ProgressLog() {
}

void ProgressLog::update(const sf::Time& frameTime) {
	auto it = m_logTexts.begin();
	while (it != m_logTexts.end()) {
		(*it).second = (*it).second - frameTime;
		if ((*it).second <= sf::Time::Zero) {
			it = m_logTexts.erase(it);
			calculatePositions();
		}
		else {
			it++;
		}
	}
}

void ProgressLog::render(sf::RenderTarget& renderTarget) {
	for (auto& it : m_logTexts) {
		renderTarget.draw(it.first);
	}
}

void ProgressLog::addItemProgress(const std::string& itemID, int amount) {
	BitmapText progress;
	progress.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	progress.setColor(amount < 0 ? sf::Color::Red : sf::Color::Green);
	std::string text = std::to_string(amount) + "x ";
	text.append(itemID.compare("gold") == 0 ? g_textProvider->getText("Gold") : g_textProvider->getText(itemID));
	progress.setString(text);
	m_logTexts.push_back(std::make_pair(progress, TIME_TO_LIVE));
	calculatePositions();
}

void ProgressLog::addQuestConditionFullfilled(const std::string& questID, const std::string& condition) {
	if (m_core->getQuestState(questID) != QuestState::Started) return;

	BitmapText progress;
	progress.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	progress.setColor(sf::Color::Cyan);
	std::string progressText = g_textProvider->getText(questID) + ":\n";
	progressText.append(g_textProvider->getText(condition) + " " + g_textProvider->getText("Done"));
	progress.setString(progressText);
	m_logTexts.push_back(std::make_pair(progress, TIME_TO_LIVE));
	calculatePositions();
}

void ProgressLog::addQuestTargetKilled(const std::string& questID, const std::string& name) {
	if (m_core->getQuestState(questID) != QuestState::Started) return;

	BitmapText questName;
	questName.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	questName.setColor(sf::Color::Green);
	questName.setString("\"" + g_textProvider->getText(questID) + "\"");

	std::string target = g_textProvider->getText(name);
	target.append(": ");
	int progress = m_core->getNumberOfTargetsKilled(questID, name);
	int goal = m_core->getNumberOfTotalTargets(questID, name);
	if (m_core->getQuestData(questID) != nullptr) {
		const QuestData* data = m_core->getQuestData(questID);
		if (data->targets.find(name) != data->targets.end())
			goal = data->targets.at(name);
	}
	if (m_core->getData().questTargetProgress.find(questID) != m_core->getData().questTargetProgress.end() &&
		m_core->getData().questTargetProgress.at(questID).find(name) != m_core->getData().questTargetProgress.at(questID).end()) {
		progress = m_core->getData().questTargetProgress.at(questID).at(name);
	}
	target.append(std::to_string(progress) + "/" + std::to_string(goal));

	// identation
	int spacesToAdd = (int)(questName.getString().getSize() - target.size()) / 2;
	for (int i = 0; i < spacesToAdd; i++) {
		target.append(" ");
	}

	BitmapText targetText;
	targetText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	targetText.setString(target);
	targetText.setColor(progress >= goal ? sf::Color::Green : sf::Color::White);
	targetText.setString(target);

	m_logTexts.push_back(std::make_pair(questName, TIME_TO_LIVE));
	m_logTexts.push_back(std::make_pair(targetText, TIME_TO_LIVE));
	calculatePositions();
}

void ProgressLog::addQuestStateChanged(const std::string& questID, QuestState state) {
	BitmapText progress;
	progress.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	progress.setColor(state == QuestState::Completed ? sf::Color::Green : state == QuestState::Failed ? sf::Color::Red : sf::Color::Yellow);
	std::string progressText = g_textProvider->getText(questID) + ": ";
	progressText.append(g_textProvider->getText(EnumNames::getQuestStateName(state)));
	progress.setString(progressText);
	m_logTexts.push_back(std::make_pair(progress, TIME_TO_LIVE));
	calculatePositions();
}

void ProgressLog::calculatePositions() {
	float yOffset = YOFFSET;
	for (auto& it : m_logTexts) {
		it.first.setPosition(sf::Vector2f(
			WINDOW_WIDTH - it.first.getLocalBounds().width - XOFFSET, yOffset));
		yOffset += it.first.getLocalBounds().height + 0.5f * GUIConstants::CHARACTER_SIZE_M;
	}
}