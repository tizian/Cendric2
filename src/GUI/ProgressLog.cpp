#include "GUI/ProgressLog.h"
#include "CharacterCore.h"
#include "Item.h"
#include "GlobalResource.h"

/* ProgressLog */

ProgressLog::ProgressLog(const CharacterCore* core) : m_core(core) {
}

ProgressLog::~ProgressLog() {
	for (auto it : m_logTexts) {
		delete it;
	}
}

void ProgressLog::update(const sf::Time& frameTime) {
	bool recalculate = false;
	auto it = m_logTexts.begin();
	while (it != m_logTexts.end()) {
		ProgressLogEntry* entry = (*it);
		entry->update(frameTime);
		if (entry->getTime() < sf::Time::Zero) {
			delete entry;
			it = m_logTexts.erase(it);
			recalculate = true;
		}
		else {
			++it;
		}
	}

	if (recalculate) {
		calculatePositions();
	}
}

void ProgressLog::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	for (auto it : m_logTexts) {
		it->render(renderTarget);
	}
}

void ProgressLog::addItemProgress(const std::string& itemID, int amount) {
	std::string text = std::to_string(amount) + "x ";
	text.append(itemID.compare("gold") == 0 ? g_textProvider->getText("Gold") : g_textProvider->getText(itemID, "item"));
	m_logTexts.push_back(new ProgressLogEntry(text, amount < 0 ? COLOR_BAD : COLOR_GOOD, itemID));

	calculatePositions();
}

void ProgressLog::addPermanentItemProgress(const Item& item) {
	std::string text = g_textProvider->getText(item.getID(), "item") + " ";
	text.append(g_textProvider->getText("Consumed"));
	m_logTexts.push_back(new ProgressLogEntry(text, COLOR_GOOD));

	text.clear();
	AttributeData::appendAttributes(text, item.getAttributes());
	m_logTexts.push_back(new ProgressLogEntry(text, COLOR_WHITE));
	
	calculatePositions();
}

void ProgressLog::addQuestConditionFullfilled(const std::string& questID, const std::string& condition) {
	if (m_core->getQuestState(questID) != QuestState::Started) return;

	std::string text = g_textProvider->getText(questID, "quest") + ":\n";
	text.append(g_textProvider->getText(condition, "quest_condition") + " " + g_textProvider->getText("Done"));
	m_logTexts.push_back(new ProgressLogEntry(text, sf::Color::Cyan));

	calculatePositions();
}

void ProgressLog::addQuestTargetKilled(const std::string& questID, const std::string& name) {
	if (m_core->getQuestState(questID) != QuestState::Started) return;

	std::string questName = "\"" + g_textProvider->getText(questID, "quest") + "\"";

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
	int spacesToAdd = (int)(questName.size() - target.size()) / 2;
	for (int i = 0; i < spacesToAdd; i++) {
		target.append(" ");
	}

	m_logTexts.push_back(new ProgressLogEntry(questName, COLOR_GOOD));
	m_logTexts.push_back(new ProgressLogEntry(target, progress >= goal ? COLOR_GOOD : COLOR_WHITE));

	calculatePositions();
}

void ProgressLog::addQuestStateChanged(const std::string& questID, QuestState state) {
	std::string text = g_textProvider->getText(questID,  "quest") + ": ";
	text.append(g_textProvider->getText(EnumNames::getQuestStateName(state)));
	m_logTexts.push_back(new ProgressLogEntry(text, state == QuestState::Completed ? COLOR_GOOD : state == QuestState::Failed ? COLOR_BAD : COLOR_NEUTRAL));

	calculatePositions();
}

void ProgressLog::addQuestDescriptionAdded(const std::string& questID) {
	std::string text = g_textProvider->getText(questID, "quest") + ": ";
	text.append(g_textProvider->getText("NewJournalEntry"));
	m_logTexts.push_back(new ProgressLogEntry(text, COLOR_GOOD));

	calculatePositions();
}

void ProgressLog::addReputationAdded(FractionID fraction, int amount) {
	if (amount <= 0) return;

	std::string text = g_textProvider->getText(EnumNames::getFractionIDName(fraction)) + ": ";
	text.append("+" + std::to_string(amount));
	m_logTexts.push_back(new ProgressLogEntry(text, COLOR_GOOD));

	calculatePositions();
}

void ProgressLog::setYOffset(float yOffset) {
	m_yOffset = yOffset;
}

void ProgressLog::setVisible(bool visible) {
	m_isVisible = visible;
}

void ProgressLog::calculatePositions() {
	float y = WINDOW_HEIGHT - m_yOffset;
	for (size_t i = 0; i < m_logTexts.size(); ++i) {
		m_logTexts[i]->setPosition(sf::Vector2f(XOFFSET, y));
		if (i < m_logTexts.size() - 1) {
			y -= m_logTexts[i + 1]->getHeight() + ProgressLogEntry::ENTRY_SPACING;
		}
	}
}

/* ProgressLogEntry */

const float ProgressLogEntry::TIME_TO_LIVE = 5.f;
const float ProgressLogEntry::BORDER_SIZE = 31.f;
const float ProgressLogEntry::ICON_SIZE = 25.f;
const float ProgressLogEntry::ICON_OFFSET = 3.f;
const float ProgressLogEntry::ICON_MARGIN = 10.f;
const float ProgressLogEntry::ENTRY_SPACING = 8.f;

ProgressLogEntry::ProgressLogEntry(const std::string& str, const sf::Color& color) {
	m_time = sf::seconds(TIME_TO_LIVE);

	m_text = new BitmapText();
	m_text->setTextStyle(TextStyle::Shadowed);
	m_text->setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_text->setColor(color);
	m_text->setString(str);
}
ProgressLogEntry::ProgressLogEntry(const std::string& str, const sf::Color& color, const std::string &itemID) {
	m_time = sf::seconds(TIME_TO_LIVE);

	m_text = new BitmapText();
	m_text->setTextStyle(TextStyle::Shadowed);
	m_text->setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_text->setColor(color);
	m_text->setString(str);

	m_icon = new sf::RectangleShape();
	m_icon->setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_icon->setTexture(g_resourceManager->getTexture(GlobalResource::TEX_ITEMS));
	Item item(itemID);
	sf::IntRect textureRect(item.getIconTextureLocation().x, item.getIconTextureLocation().y, 50, 50);
	m_icon->setTextureRect(textureRect);

	m_background = new sf::RectangleShape();
	m_background->setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_background->setFillColor(COLOR_TRANS_GREY);

	m_border = new sf::RectangleShape();
	m_border->setSize(sf::Vector2f(BORDER_SIZE, BORDER_SIZE));
	m_border->setTexture(g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_PROGRESS_LOG));
}

ProgressLogEntry::~ProgressLogEntry() {
	delete m_text;
	if (m_icon) {
		delete m_icon;
		delete m_background;
		delete m_border;
	}
}

void ProgressLogEntry::update(const sf::Time& frameTime) {
	m_time -= frameTime;
}

void ProgressLogEntry::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(*m_text);
	if (m_icon) {
		renderTarget.draw(*m_background);
		renderTarget.draw(*m_icon);
		renderTarget.draw(*m_border);
	}
}

void ProgressLogEntry::setPosition(const sf::Vector2f& position) {
	float textHeight = m_text->getLocalBounds().height;
	if (m_icon) {
		m_border->setPosition(position);
		m_text->setPosition(position.x + BORDER_SIZE + ICON_MARGIN, position.y + 0.5f * (BORDER_SIZE - textHeight));

		m_background->setPosition(position.x + ICON_OFFSET, position.y + ICON_OFFSET);
		m_icon->setPosition(position.x + ICON_OFFSET, position.y + ICON_OFFSET);
	}
	else {
		m_text->setPosition(position);
	}
}

float ProgressLogEntry::getHeight() const {
	if (m_icon) return BORDER_SIZE;
	else return m_text->getLocalBounds().height;
}