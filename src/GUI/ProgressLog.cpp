#include "GUI/ProgressLog.h"
#include "CharacterCore.h"
#include "World/Item.h"
#include "GlobalResource.h"

/* ProgressLog */

const float ProgressLog::XOFFSET = 20.f;
const int ProgressLog::MAX_ENTRIES = 6;

ProgressLog::ProgressLog(const CharacterCore* core) : m_core(core) {
}

ProgressLog::~ProgressLog() {
	CLEAR_VECTOR(m_logTexts);
}

void ProgressLog::update(const sf::Time& frameTime) {
	auto it = m_logTexts.begin();
	while (it != m_logTexts.end()) {
		ProgressLogEntry* entry = (*it);
		entry->update(frameTime);
		++it;
	}


	it = m_logTexts.begin();
	if (it == m_logTexts.end()) return;

	ProgressLogEntry* entry = (*it);
	entry->updateBottom(frameTime);
	if (entry->isOver()) {
		delete entry;
		it = m_logTexts.erase(it);
	}

	calculatePositions();
}

void ProgressLog::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	renderTarget.setView(renderTarget.getDefaultView());
	for (auto it : m_logTexts) {
		it->render(renderTarget);
	}
}

void ProgressLog::addItemProgress(const std::string& itemID, int amount) {
	std::string text = std::to_string(amount) + "x ";
	text.append(itemID.compare("gold") == 0 ? g_textProvider->getText("Gold") : g_textProvider->getText(itemID, "item"));

	m_logTexts.push_back(ProgressLogEntry::createItemEntry(text, amount < 0 ? COLOR_MEDIUM_GREY : COLOR_WHITE, itemID));

	calculatePositions();
}

void ProgressLog::addItemConversionProgress(const std::string& oldItemID, const std::string& newItemID) {
	std::string text = g_textProvider->getText(oldItemID, "item")
		+ " -> "
		+ g_textProvider->getText(newItemID, "item");

	m_logTexts.push_back(ProgressLogEntry::createItemEntry(text, COLOR_WHITE, newItemID));

	calculatePositions();
}

void ProgressLog::addPermanentItemProgress(const Item* item) {
	if (item == nullptr) return;
	std::string text = g_textProvider->getText(item->getID(), "item") + " ";
	text.append(g_textProvider->getText("Consumed"));
	text.append("\n");
	AttributeData::appendAttributes(text, item->getAttributes());

	m_logTexts.push_back(ProgressLogEntry::createItemEntry(text, COLOR_NEUTRAL, item->getID()));

	calculatePositions();
}

void ProgressLog::addQuestConditionFullfilled(const std::string& questID, const std::string& condition) {
	if (m_core->getQuestState(questID) != QuestState::Started) return;

	std::string text = g_textProvider->getText(questID, "quest") + ":\n";
	text.append(g_textProvider->getText(condition, "quest_condition") + " " + g_textProvider->getText("Done"));
	m_logTexts.push_back(ProgressLogEntry::createQuestEntry(text, COLOR_WHITE));

	calculatePositions();
}

void ProgressLog::addQuestItemCollected(const std::string& questID, const std::string& itemID, int currentAmount, int goalAmount) {
	if (m_core->getQuestState(questID) != QuestState::Started) return;

	std::string text = g_textProvider->getText(itemID, "item");
	text.append(" (");
	text.append(std::to_string(std::min(currentAmount, goalAmount)) + "/" + std::to_string(goalAmount) + ")");

	m_logTexts.push_back(ProgressLogEntry::createQuestEntry(text, COLOR_WHITE));

	calculatePositions();
}

void ProgressLog::addQuestTargetKilled(const std::string& questID, const std::string& name) {
	if (m_core->getQuestState(questID) != QuestState::Started) return;

	std::string text = g_textProvider->getText(name, "enemy");
	text.append(" (");
	int progress = m_core->getNumberOfTargetsKilled(questID, name);
	int goal = m_core->getNumberOfTotalTargets(questID, name);
	if (m_core->getQuestData(questID) != nullptr) {
		const QuestData* data = m_core->getQuestData(questID);
		if (contains(data->targets, name))
			goal = data->targets.at(name);
	}
	if (contains(m_core->getData().questTargetProgress, questID) &&
		contains(m_core->getData().questTargetProgress.at(questID), name)) {
		progress = m_core->getData().questTargetProgress.at(questID).at(name);
	}
	text.append(std::to_string(progress) + "/" + std::to_string(goal) + ")");

	m_logTexts.push_back(ProgressLogEntry::createQuestEntry(text, COLOR_WHITE));

	calculatePositions();
}

void ProgressLog::addQuestStateChanged(const std::string& questID, QuestState state) {
	std::string text = g_textProvider->getText(questID, "quest") + ": ";
	text.append(g_textProvider->getText(EnumNames::getQuestStateName(state)));
	m_logTexts.push_back(ProgressLogEntry::createQuestEntry(text, state == QuestState::Completed ? COLOR_GOOD : state == QuestState::Failed ? COLOR_BAD : COLOR_NEUTRAL));
	g_resourceManager->playSound(GlobalResource::SOUND_GUI_QUESTPROGRESS);

	calculatePositions();
}

void ProgressLog::addQuestDescriptionAdded(const std::string& questID) {
	if (m_core->getQuestState(questID) != QuestState::Started) return;
	std::string text = g_textProvider->getText(questID, "quest") + ": ";
	text.append(g_textProvider->getText("NewJournalEntry"));
	m_logTexts.push_back(ProgressLogEntry::createQuestEntry(text, COLOR_NEUTRAL));
	g_resourceManager->playSound(GlobalResource::SOUND_GUI_QUESTPROGRESS);

	calculatePositions();
}

void ProgressLog::addReputationAdded(FractionID fraction, int amount) {
	if (amount <= 0) return;

	std::string text = g_textProvider->getText(EnumNames::getShortFractionIDName(fraction)) + ": ";
	text.append("+" + std::to_string(amount));
	m_logTexts.push_back(ProgressLogEntry::createReputationEntry(text, COLOR_NEUTRAL));

	calculatePositions();
}

void ProgressLog::addGuildJoined(FractionID fraction) {
	std::string text = g_textProvider->getText("GuildJoined");
	text.append(": ");
	text.append(g_textProvider->getText(EnumNames::getFractionIDName(fraction)));

	m_logTexts.push_back(ProgressLogEntry::createReputationEntry(text, COLOR_NEUTRAL));

	calculatePositions();
}

void ProgressLog::setYOffset(float yOffset) {
	m_yOffset = yOffset;
}

void ProgressLog::setVisible(bool visible) {
	m_isVisible = visible;
}

void ProgressLog::calculatePositions() {
	if (m_logTexts.size() == 0) return;
	while (m_logTexts.size() > MAX_ENTRIES) {
		delete m_logTexts.at(0);
		m_logTexts.erase(m_logTexts.begin());
	}

	float y = WINDOW_HEIGHT - m_yOffset;

	float time = m_logTexts[0]->getScrollTime().asSeconds();
	float start = 0.f;
	float change = m_logTexts[0]->getHeight() + ProgressLogEntry::ENTRY_SPACING;
	float animationTime = ProgressLogEntry::TIME_TO_SCROLL.asSeconds();
	float scrollOffset = easeInOutQuad(time, start, change, animationTime);

	for (size_t i = 0; i < m_logTexts.size(); ++i) {
		m_logTexts[i]->setPosition(sf::Vector2f(XOFFSET, y + scrollOffset));
		if (i < m_logTexts.size() - 1) {
			y -= m_logTexts[i + 1]->getHeight() + ProgressLogEntry::ENTRY_SPACING;
		}
	}


}

/* ProgressLogEntry */

const sf::Time ProgressLogEntry::TIME_TO_LIVE = sf::seconds(6.f);
const sf::Time ProgressLogEntry::TIME_TO_FADE = sf::seconds(0.2f);
const sf::Time ProgressLogEntry::TIME_TO_SCROLL = sf::seconds(0.3f);

const float ProgressLogEntry::BORDER_SIZE = 31.f;
const float ProgressLogEntry::ICON_SIZE = 25.f;
const float ProgressLogEntry::ICON_OFFSET = 3.f;
const float ProgressLogEntry::ICON_MARGIN = 10.f;
const float ProgressLogEntry::ENTRY_SPACING = 8.f;

ProgressLogEntry::ProgressLogEntry() {
	m_time = TIME_TO_LIVE;
	m_fadeInTimer = TIME_TO_FADE;
	m_fadeOutTimer = TIME_TO_FADE;
	m_scrollTimer = TIME_TO_SCROLL;

	m_text = new BitmapText();
	m_text->setTextStyle(TextStyle::Shadowed);
	m_text->setCharacterSize(GUIConstants::CHARACTER_SIZE_M);

	m_icon = new sf::RectangleShape();
	m_icon->setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));

	m_background = new sf::RectangleShape();
	m_background->setSize(sf::Vector2f(ICON_SIZE, ICON_SIZE));
	m_background->setFillColor(COLOR_TRANS_GREY);

	m_border = new sf::RectangleShape();
	m_border->setSize(sf::Vector2f(BORDER_SIZE, BORDER_SIZE));
	m_border->setTexture(g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLOT_PROGRESS_LOG));

	setAlpha(0.f);
}

ProgressLogEntry* ProgressLogEntry::createItemEntry(const std::string& str, const sf::Color& color, const std::string& itemID) {
	ProgressLogEntry* entry = new ProgressLogEntry();

	entry->m_text->setColor(color);
	entry->m_text->setString(str);
	entry->setAlpha(0.f);

	if (itemID.compare("gold") == 0) {
		entry->m_icon->setTexture(g_resourceManager->getTexture(GlobalResource::TEX_GUI_PROGRESSLOG_ICONS));
		entry->m_icon->setTextureRect(sf::IntRect(0, 0, 25, 25));
		return entry;
	}
	entry->m_icon->setTexture(g_resourceManager->getTexture(GlobalResource::TEX_ITEMS));
	Item* item = g_resourceManager->getItem(itemID);
	if (item == nullptr) {
		return entry;
	}
	sf::IntRect textureRect(item->getIconTextureLocation().x, item->getIconTextureLocation().y, 50, 50);
	entry->m_icon->setTextureRect(textureRect);

	return entry;
}

ProgressLogEntry* ProgressLogEntry::createQuestEntry(const std::string& str, const sf::Color& color) {
	ProgressLogEntry* entry = new ProgressLogEntry();

	entry->m_text->setColor(color);
	entry->m_text->setString(str);
	entry->setAlpha(0.f);

	entry->m_icon->setTexture(g_resourceManager->getTexture(GlobalResource::TEX_GUI_PROGRESSLOG_ICONS));
	entry->m_icon->setTextureRect(sf::IntRect(25, 0, 25, 25));

	return entry;
}

ProgressLogEntry* ProgressLogEntry::createReputationEntry(const std::string& str, const sf::Color& color) {
	ProgressLogEntry* entry = new ProgressLogEntry();

	entry->m_text->setColor(color);
	entry->m_text->setString(str);

	entry->m_icon->setTexture(g_resourceManager->getTexture(GlobalResource::TEX_GUI_PROGRESSLOG_ICONS));
	entry->m_icon->setTextureRect(sf::IntRect(50, 0, 25, 25));

	return entry;
}

ProgressLogEntry::~ProgressLogEntry() {
	delete m_text;
	delete m_icon;
	delete m_background;
	delete m_border;
}

void ProgressLogEntry::update(const sf::Time& frameTime) {
	if (m_fadeInTimer > sf::Time::Zero) {
		updateTime(m_fadeInTimer, frameTime);
		float scale = 1.f - m_fadeInTimer.asSeconds() / TIME_TO_FADE.asSeconds();
		setAlpha(scale);
	}
	else {
		updateTime(m_time, frameTime);
	}
}

void ProgressLogEntry::updateBottom(const sf::Time& frameTime) {
	if (m_time > sf::Time::Zero) return;

	if (m_fadeOutTimer > sf::Time::Zero) {
		updateTime(m_fadeOutTimer, frameTime);
		float scale = m_fadeOutTimer.asSeconds() / TIME_TO_FADE.asSeconds();
		setAlpha(scale);
	}
	else {
		updateTime(m_scrollTimer, frameTime);
	}
}

void ProgressLogEntry::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(*m_text);
	renderTarget.draw(*m_background);
	renderTarget.draw(*m_icon);
	renderTarget.draw(*m_border);
}

void ProgressLogEntry::setPosition(const sf::Vector2f& position) {
	float textHeight = m_text->getLocalBounds().height;
	m_border->setPosition(position);
	m_text->setPosition(position.x + BORDER_SIZE + ICON_MARGIN, position.y + 0.5f * (BORDER_SIZE - textHeight));

	m_background->setPosition(position.x + ICON_OFFSET, position.y + ICON_OFFSET);
	m_icon->setPosition(position.x + ICON_OFFSET, position.y + ICON_OFFSET);
}

void ProgressLogEntry::setAlpha(float alpha) {
	sf::Uint8 a = (sf::Uint8)(alpha * 255);
	sf::Uint8 aBackground = (sf::Uint8)(COLOR_TRANS_GREY.a * alpha);

	const sf::Color &tc = m_text->getColor();
	m_text->setColor(sf::Color(tc.r, tc.g, tc.b, a));

	const sf::Color &ic = m_icon->getFillColor();
	m_icon->setFillColor(sf::Color(ic.r, ic.g, ic.b, a));

	const sf::Color &bgc = m_background->getFillColor();
	m_background->setFillColor(sf::Color(bgc.r, bgc.g, bgc.b, aBackground));

	const sf::Color &bdc = m_border->getFillColor();
	m_border->setFillColor(sf::Color(bdc.r, bdc.g, bdc.b, a));
}

sf::Time ProgressLogEntry::getScrollTime() const {
	return TIME_TO_SCROLL - m_scrollTimer;
}

float ProgressLogEntry::getHeight() const {
	float textHeight = m_text->getLocalBounds().height;
	if (textHeight > BORDER_SIZE) return textHeight;
	else return BORDER_SIZE;
}