#include "GUI/ProgressLog.h"
#include "CharacterCore.h"
#include "Item.h"

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
	progress.setTextStyle(TextStyle::Shadowed);
	progress.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	progress.setColor(amount < 0 ? COLOR_BAD : COLOR_GOOD);
	std::string text = std::to_string(amount) + "x ";
	text.append(itemID.compare("gold") == 0 ? g_textProvider->getText("Gold") : g_textProvider->getText(itemID, "item"));
	progress.setString(text);
	m_logTexts.push_back(std::make_pair(progress, TIME_TO_LIVE));
	calculatePositions();
}

void ProgressLog::addPermanentItemProgress(const Item& item) {
	BitmapText progress;
	progress.setTextStyle(TextStyle::Shadowed);
	progress.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	progress.setColor(COLOR_GOOD);

	std::string text = g_textProvider->getText(item.getID(), "item") + " ";
	text.append(g_textProvider->getText("Consumed"));
	progress.setString(text);

	m_logTexts.push_back(std::make_pair(progress, TIME_TO_LIVE));

	progress.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	progress.setColor(COLOR_WHITE);
	text.clear();
	AttributeData::appendAttributes(text, item.getAttributes());
	progress.setString(text);

	m_logTexts.push_back(std::make_pair(progress, TIME_TO_LIVE));
	
	calculatePositions();
}

void ProgressLog::addQuestConditionFullfilled(const std::string& questID, const std::string& condition) {
	if (m_core->getQuestState(questID) != QuestState::Started) return;

	BitmapText progress;
	progress.setTextStyle(TextStyle::Shadowed);
	progress.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	progress.setColor(sf::Color::Cyan);
	std::string progressText = g_textProvider->getText(questID, "quest") + ":\n";
	progressText.append(g_textProvider->getText(condition, "quest_condition") + " " + g_textProvider->getText("Done"));
	progress.setString(progressText);
	m_logTexts.push_back(std::make_pair(progress, TIME_TO_LIVE));
	calculatePositions();
}

void ProgressLog::addQuestTargetKilled(const std::string& questID, const std::string& name) {
	if (m_core->getQuestState(questID) != QuestState::Started) return;

	BitmapText questName;
	questName.setTextStyle(TextStyle::Shadowed);
	questName.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	questName.setColor(COLOR_GOOD);
	questName.setString("\"" + g_textProvider->getText(questID, "quest") + "\"");

	std::string target = g_textProvider->getText(name, "enemy");
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
	int spacesToAdd = (int)(questName.getString().size() - target.size()) / 2;
	for (int i = 0; i < spacesToAdd; i++) {
		target.append(" ");
	}

	BitmapText targetText;
	targetText.setTextStyle(TextStyle::Shadowed);
	targetText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	targetText.setString(target);
	targetText.setColor(progress >= goal ? COLOR_GOOD : COLOR_WHITE);
	targetText.setString(target);

	m_logTexts.push_back(std::make_pair(questName, TIME_TO_LIVE));
	m_logTexts.push_back(std::make_pair(targetText, TIME_TO_LIVE));
	calculatePositions();
}

void ProgressLog::addQuestStateChanged(const std::string& questID, QuestState state) {
	BitmapText progress;
	progress.setTextStyle(TextStyle::Shadowed);
	progress.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	progress.setColor(state == QuestState::Completed ? COLOR_GOOD : state == QuestState::Failed ? COLOR_BAD : COLOR_NEUTRAL);
	std::string progressText = g_textProvider->getText(questID,  "quest") + ": ";
	progressText.append(g_textProvider->getText(EnumNames::getQuestStateName(state)));
	progress.setString(progressText);
	m_logTexts.push_back(std::make_pair(progress, TIME_TO_LIVE));
	calculatePositions();
}

void ProgressLog::addQuestDescriptionAdded(const std::string& questID) {
	BitmapText progress;
	progress.setTextStyle(TextStyle::Shadowed);
	progress.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	progress.setColor(COLOR_GOOD);
	std::string progressText = g_textProvider->getText(questID, "quest") + ": ";
	progressText.append(g_textProvider->getText("NewJournalEntry"));
	progress.setString(progressText);
	m_logTexts.push_back(std::make_pair(progress, TIME_TO_LIVE));
	calculatePositions();
}

void ProgressLog::addReputationAdded(FractionID fraction, int amount) {
	if (amount <= 0) return;
	BitmapText progress;
	progress.setTextStyle(TextStyle::Shadowed);
	progress.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	progress.setColor(COLOR_GOOD);
	std::string progressText = g_textProvider->getText(EnumNames::getFractionIDName(fraction)) + ": ";
	progressText.append("+" + std::to_string(amount));
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