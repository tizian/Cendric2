#include "GUI/QuestLog.h"
#include "GUI/QuestDescriptionWindow.h"
#include "GUI/ScrollBar.h"
#include "GUI/ScrollHelper.h"
#include "GlobalResource.h"

// <<< QUEST LOG >>>

const int QuestLog::ENTRY_COUNT = 17;
const int QuestLog::MAX_ENTRY_LENGTH_CHARACTERS = 34;
const float QuestLog::MAX_ENTRY_LENGTH = static_cast<float>(MAX_ENTRY_LENGTH_CHARACTERS) * GUIConstants::CHARACTER_SIZE_M;

const float QuestLog::WINDOW_MARGIN = 6.f;

const float QuestLog::SCROLL_WINDOW_LEFT = GUIConstants::TEXT_OFFSET;
const float QuestLog::SCROLL_WINDOW_WIDTH = MAX_ENTRY_LENGTH + 4 * WINDOW_MARGIN + ScrollBar::WIDTH;
const float QuestLog::SCROLL_WINDOW_HEIGHT = ENTRY_COUNT * GUIConstants::CHARACTER_SIZE_M + (ENTRY_COUNT - 1) * GUIConstants::CHARACTER_SIZE_M + 2 * GUIConstants::CHARACTER_SIZE_M + 2 * WINDOW_MARGIN;

const float QuestLog::TOP = GUIConstants::TOP;
const float QuestLog::LEFT = GUIConstants::LEFT;
const float QuestLog::WIDTH = SCROLL_WINDOW_WIDTH + 2 * SCROLL_WINDOW_LEFT;

const sf::Vector2f QuestLog::BUTTON_SIZE = sf::Vector2f(SCROLL_WINDOW_WIDTH / 3.f, 40.f);

const float QuestLog::SCROLL_WINDOW_TOP = GUIConstants::GUI_TABS_TOP + 2 * WINDOW_MARGIN + BUTTON_SIZE.y;

QuestLog::QuestLog(CharacterCore* core) {
	m_core = core;

	init();
}

void QuestLog::init() {
	// init window
	sf::FloatRect box(LEFT, TOP, WIDTH, GUIConstants::GUI_WINDOW_HEIGHT);
	m_window = new Window(box,
		GUIOrnamentStyle::LARGE,
		GUIConstants::MAIN_COLOR,
		GUIConstants::ORNAMENT_COLOR);

	m_window->addCloseButton(std::bind(&QuestLog::hide, this));

	delete m_descriptionWindow;
	m_descriptionWindow = new QuestDescriptionWindow(m_core);

	// init text
	m_title.setPosition(sf::Vector2f(LEFT + GUIConstants::TEXT_OFFSET, TOP + GUIConstants::TEXT_OFFSET));
	m_title.setColor(COLOR_WHITE);
	m_title.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_title.setString(g_textProvider->getText("Journal"));
	m_title.setPosition(
		m_window->getPosition().x +
		WIDTH / 2 -
		m_title.getLocalBounds().width / 2, m_title.getPosition().y);

	// fill the helper map
	m_stateMap.insert({
		{ QuestState::Started, &m_startedQuests },
		{ QuestState::Completed, &m_completedQuests },
		{ QuestState::Failed, &m_failedQuests },
	});

	// init tabbar
	int nTabs = 3;
	float width = nTabs * BUTTON_SIZE.x;
	float height = BUTTON_SIZE.y;
	float x = LEFT + 0.5f * (WIDTH - width);
	float y = TOP + GUIConstants::GUI_TABS_TOP;
	
	m_tabBar = new TabBar(sf::FloatRect(x, y, width, height), nTabs);
	for (int i = 0; i < nTabs; ++i) {
		m_tabBar->getTabButton(i)->setText(EnumNames::getQuestStateName((QuestState)(i+1)));
		m_tabBar->getTabButton(i)->setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	}

	// init scrolling
	m_scrollWindow = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_ORNAMENT_NONE), COLOR_WHITE, SCROLL_WINDOW_WIDTH, SCROLL_WINDOW_HEIGHT);
	m_scrollWindow.setPosition(sf::Vector2f(LEFT + SCROLL_WINDOW_LEFT, TOP + SCROLL_WINDOW_TOP));

	m_scrollBar = new ScrollBar(SCROLL_WINDOW_HEIGHT, m_window);
	m_scrollBar->setPosition(sf::Vector2f(LEFT + SCROLL_WINDOW_LEFT + SCROLL_WINDOW_WIDTH - ScrollBar::WIDTH, TOP + SCROLL_WINDOW_TOP));

	sf::FloatRect scrollBox(LEFT + SCROLL_WINDOW_LEFT, TOP + SCROLL_WINDOW_TOP, SCROLL_WINDOW_WIDTH, SCROLL_WINDOW_HEIGHT);
	m_scrollHelper = new ScrollHelper(scrollBox);

	// init empty text
	m_emptyText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_emptyText.setString(g_textProvider->getText("NoQuests"));
	const sf::FloatRect bounds = m_emptyText.getBounds();
	m_emptyText.setPosition(scrollBox.left + 0.5f * (scrollBox.width - bounds.width), scrollBox.top + 0.5f * (scrollBox.height - bounds.height));

	reload();

	selectTab(QuestState::Started);
}

QuestLog::~QuestLog() {
	delete m_window;
	delete m_descriptionWindow;
	delete m_tabBar;
	delete m_scrollBar;
	delete m_scrollHelper;
	clearAllEntries();
}

void QuestLog::clearAllEntries() {
	m_completedQuests.clear();
	m_failedQuests.clear();
	m_startedQuests.clear();
	m_selectedEntry = nullptr;
}

void QuestLog::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	m_scrollBar->update(frameTime);

	// check whether an entry was selected
	for (auto& it : *(m_stateMap[m_currentTab])) {
		sf::Vector2f pos = it.getPosition();
		if (pos.y < TOP + SCROLL_WINDOW_TOP ||
			pos.y + GUIConstants::CHARACTER_SIZE_M > TOP + SCROLL_WINDOW_TOP + SCROLL_WINDOW_HEIGHT) continue;
		it.update(frameTime);
		if (it.isClicked()) {
			selectEntry(&it);
			return;
		}
	}

	m_tabBar->update(frameTime);
	int activeIndex = m_tabBar->getActiveTabIndex();
	QuestState state = (QuestState)(activeIndex + 1);
	if (m_tabBar->getTabButton(activeIndex)->isClicked() && m_currentTab != state) {
		selectTab(state);
	}

	calculateEntryPositions();

	m_window->update(frameTime);
}

void QuestLog::calculateEntryPositions() {
	std::vector<QuestEntry>* entries = m_stateMap[m_currentTab];

	int rows = static_cast<int>(entries->size());
	int steps = rows - ENTRY_COUNT + 1;

	m_scrollBar->setDiscreteSteps(steps);

	int scrollPos = m_scrollBar->getDiscreteScrollPosition();

	if (2.f * scrollPos * GUIConstants::CHARACTER_SIZE_M != m_scrollHelper->nextOffset) {
		m_scrollHelper->lastOffset = m_scrollHelper->nextOffset;
		m_scrollHelper->nextOffset = 2.f * scrollPos * GUIConstants::CHARACTER_SIZE_M;
	}

	float animationTime = 0.1f;
	float time = m_scrollBar->getScrollTime().asSeconds();
	if (time >= animationTime) {
		m_scrollHelper->lastOffset = m_scrollHelper->nextOffset;
	}
	float start = m_scrollHelper->lastOffset;
	float change = m_scrollHelper->nextOffset - m_scrollHelper->lastOffset;
	float effectiveScrollOffset = easeInOutQuad(time, start, change, animationTime);

	float xOffset = LEFT + SCROLL_WINDOW_LEFT + 2 * WINDOW_MARGIN;
	float yOffset = TOP + SCROLL_WINDOW_TOP + WINDOW_MARGIN + GUIConstants::CHARACTER_SIZE_M - effectiveScrollOffset;

	for (auto& it : *entries) {
		it.setBoundingBox(sf::FloatRect(xOffset, yOffset + 0.5f * GUIConstants::CHARACTER_SIZE_M, SCROLL_WINDOW_WIDTH - ScrollBar::WIDTH, 2.f * GUIConstants::CHARACTER_SIZE_M));
		it.setPosition(sf::Vector2f(xOffset, yOffset));
		yOffset += 2.f * GUIConstants::CHARACTER_SIZE_M;
	}
}

void QuestLog::selectEntry(QuestEntry* entry) {
	if (entry == nullptr) return;
	if (entry == m_selectedEntry) return;
	if (m_selectedEntry != nullptr) {
		m_selectedEntry->deselect();
	}
	m_selectedEntry = entry;
	m_selectedEntry->select();
	showDescription(m_selectedEntry->getQuestID());
	m_selectedQuestID = m_selectedEntry->getQuestID();
}

bool QuestLog::isVisible() const {
	return m_isVisible;
}

void QuestLog::render(sf::RenderTarget& target) {
	if (!m_isVisible) return;

	m_window->render(target);
	target.draw(m_title);
	for (auto& it : *(m_stateMap[m_currentTab])) {
		it.render(m_scrollHelper->texture);
		// it.renderAfterForeground(target); // uncomment for debug box
	}
	m_scrollHelper->render(target);

	m_tabBar->render(target);

	m_descriptionWindow->render(target);

	target.draw(m_scrollWindow);
	m_scrollBar->render(target);

	if (m_stateMap[m_currentTab]->size() == 0) {
		target.draw(m_emptyText);
	}
}

void QuestLog::showDescription(const std::string& questID) {
	m_descriptionWindow->reload(questID);
	m_descriptionWindow->show();
}

void QuestLog::hideDescription() {
	m_descriptionWindow->hide();
	m_selectedQuestID = "";
}

void QuestLog::selectTab(QuestState state) {
	hideDescription();
	if (m_selectedEntry != nullptr) {
		m_selectedEntry->deselect();
		m_selectedEntry = nullptr;
	}
	m_currentTab = state;

	m_scrollBar->setScrollPosition(0.f);
}

void QuestLog::reload() {
	m_scrollBar->scroll(0);

	clearAllEntries();
	std::vector<std::pair<QuestState, std::string>> mainQuests;
	std::vector<std::pair<QuestState, std::string>> nonMainQuests;

	for (auto& it : m_core->getData().questStates) {
		if (m_stateMap[it.second] == nullptr) continue;
		const QuestData* data = m_core->getQuestData(it.first);
		if (!data) continue;
		if (data->isMainQuest) {
			mainQuests.push_back(std::pair<QuestState, std::string>(it.second, it.first));
		}
		else {
			nonMainQuests.push_back(std::pair<QuestState, std::string>(it.second, it.first));
		}
	}

	for (auto& it : mainQuests) {
		m_stateMap[it.first]->push_back(QuestEntry(it.second, true));
		if (it.second.compare(m_selectedQuestID) == 0 && m_currentTab != it.first) {
			// assure that an item that is not in the current tab can never be selected
			hideDescription();
		}
	}

	for (auto& it : nonMainQuests) {
		m_stateMap[it.first]->push_back(QuestEntry(it.second, false));
		if (it.second.compare(m_selectedQuestID) == 0 && m_currentTab != it.first) {
			// assure that an item that is not in the current tab can never be selected
			hideDescription();
		}
	}

	for (auto& it : m_stateMap) {
		for (auto& it2 : *it.second) {
			it2.deselect();
			if (it2.getQuestID().compare(m_selectedQuestID) == 0) {
				selectEntry(&it2);
			}
		}
	}
}

void QuestLog::show() {
	m_isVisible = true;
}

void QuestLog::hide() {
	m_isVisible = false;
}

// <<< QUEST ENTRY >>>

QuestEntry::QuestEntry(const std::string& questID, bool isMainQuest) {
	m_questID = questID;
	m_isMainQuest = isMainQuest;
	m_name.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);

	std::string questTitle = "> " + g_textProvider->getText(m_questID, "quest");
	if (questTitle.size() > QuestLog::MAX_ENTRY_LENGTH_CHARACTERS) {
		questTitle = questTitle.substr(0, QuestLog::MAX_ENTRY_LENGTH_CHARACTERS - 3) + "...";
	}
	m_name.setString(questTitle);

	setBoundingBox(sf::FloatRect(0.f, 0.f, m_name.getLocalBounds().width, m_name.getLocalBounds().height));
	setInputInDefaultView(true);
}

void QuestEntry::update(const sf::Time& frameTime) {
	m_isMouseover = false;
	m_isClicked = false;
	GameObject::update(frameTime);
	if (m_isMouseover) {
		m_name.setColor(COLOR_LIGHT_PURPLE);
	}
	else if (m_isSelected) {
		m_name.setColor(COLOR_WHITE);
	}
	else {
		m_name.setColor(m_isMainQuest ? sf::Color(220, 180, 180) : sf::Color(180, 180, 180));
	}
}

const std::string& QuestEntry::getQuestID() const {
	return m_questID;
}

void QuestEntry::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_name.setPosition(pos);
}

void QuestEntry::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_name);
}

void QuestEntry::onLeftJustPressed() {
	g_inputController->lockAction();
	m_isClicked = true;
}

void QuestEntry::onMouseOver() {
	m_isMouseover = true;
}

bool QuestEntry::isClicked() {
	return m_isClicked;
}

void QuestEntry::select() {
	m_isSelected = true;
}

GameObjectType QuestEntry::getConfiguredType() const {
	return GameObjectType::_Interface;
}

void QuestEntry::deselect() {
	m_isSelected = false;
}

bool QuestEntry::isSelected() const {
	return m_isSelected;
}