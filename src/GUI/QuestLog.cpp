#include "GUI/QuestLog.h"
#include "GUI/QuestDescriptionWindow.h"
#include "GUI/ScrollBar.h"
#include "GUI/ScrollHelper.h"
#include "GUI/WorldInterface.h"
#include "GlobalResource.h"
#include "Structs/QuestData.h"
#include "GameObjectComponents/TooltipWindowComponent.h"
#include "Screens/WorldScreen.h"

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

QuestLog::QuestLog(WorldInterface* interface) {
	m_interface = interface;
	m_core = m_interface->getCore();

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
		m_tabBar->getTabButton(i)->setText(EnumNames::getQuestStateName((QuestState)(i + 1)));
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
	CLEAR_VECTOR(m_completedQuests);
	CLEAR_VECTOR(m_failedQuests);
	CLEAR_VECTOR(m_startedQuests);
	m_selectedEntry = nullptr;
}

void QuestLog::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	m_scrollBar->update(frameTime);

	// check whether an entry was selected
	for (auto& it : *(m_stateMap[m_currentTab])) {
		sf::Vector2f pos = (*it).getPosition();
		if (pos.y < TOP + SCROLL_WINDOW_TOP ||
			pos.y + GUIConstants::CHARACTER_SIZE_M > TOP + SCROLL_WINDOW_TOP + SCROLL_WINDOW_HEIGHT) continue;
		(*it).update(frameTime);
		if ((*it).isClicked()) {
			selectEntry(it);
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
	std::vector<QuestEntry*>* entries = m_stateMap[m_currentTab];

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

	for (auto it : *entries) {
		(*it).setBoundingBox(sf::FloatRect(xOffset, yOffset + 0.5f * GUIConstants::CHARACTER_SIZE_M, SCROLL_WINDOW_WIDTH - ScrollBar::WIDTH, 2.f * GUIConstants::CHARACTER_SIZE_M));
		(*it).setPosition(sf::Vector2f(xOffset, yOffset));
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
		(*it).render(m_scrollHelper->texture);
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

void QuestLog::renderAfterForeground(sf::RenderTarget& target) {
	for (auto& it : *(m_stateMap[m_currentTab])) {
		(*it).renderAfterForeground(target);
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

		m_stateMap[it.second]->push_back(new QuestEntry(*data, m_interface, it.second == QuestState::Started));
		if (it.first == m_selectedQuestID && m_currentTab != it.second) {
			// assure that an item that is not in the current tab can never be selected
			hideDescription();
		}
	}

	for (auto& it : m_stateMap) {
		for (auto& it2 : *it.second) {
			(*it2).deselect();
			if ((*it2).getQuestID() == m_selectedQuestID) {
				selectEntry(it2);
			}
		}
	}
}

void QuestLog::notifyJumpToQuest(const std::string& questId) {
	for (auto entry : m_startedQuests)
	{ 
		if (entry->getQuestID() == questId) {
			selectEntry(entry);
			break;
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

QuestEntry::QuestEntry(const QuestData& data, WorldInterface* interface, bool isActiveQuest) {
	m_data = data;
	m_name.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);

	std::string questTitle = (isActiveQuest ? "  " : "") + g_textProvider->getText(m_data.id, "quest");
	if (questTitle.size() > QuestLog::MAX_ENTRY_LENGTH_CHARACTERS) {
		questTitle = questTitle.substr(0, QuestLog::MAX_ENTRY_LENGTH_CHARACTERS - 3) + "...";
	}
	m_name.setString(questTitle);

	setBoundingBox(sf::FloatRect(0.f, 0.f, m_name.getLocalBounds().width, m_name.getLocalBounds().height));
	setInputInDefaultView(true);

	setupQuestMarker(isActiveQuest, interface);
}

QuestEntry::~QuestEntry() {
	delete m_questMarker;
}

void QuestEntry::setupQuestMarker(bool isActiveQuest, WorldInterface* interface) {
	if (!isActiveQuest) return;
	delete m_questMarker;

	m_questMarker = new LogQuestMarker(m_data, interface);
}

void QuestEntry::update(const sf::Time& frameTime) {
	if (m_questMarker) {
		m_questMarker->update(frameTime);
	}

	m_isMouseover = false;
	m_isClicked = false;
	GameObject::update(frameTime);
	if (m_isSelected) {
		m_name.setColor(COLOR_WHITE); 
	}
	else {
		m_name.setColor(m_isMouseover ? COLOR_LIGHT_PURPLE : sf::Color(180, 180, 180));
	}
}

const std::string& QuestEntry::getQuestID() const {
	return m_data.id;
}

void QuestEntry::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_name.setPosition(pos);
	if (m_questMarker) {
		m_questMarker->setPosition(pos);
	}
}

void QuestEntry::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_name);
	if (m_questMarker) {
		m_questMarker->render(renderTarget);
	}
}

void QuestEntry::renderAfterForeground(sf::RenderTarget& renderTarget) {
	GameObject::renderAfterForeground(renderTarget);
	if (m_questMarker) {
		m_questMarker->renderAfterForeground(renderTarget);
	}
}


void QuestEntry::onLeftJustPressed() {
	g_inputController->lockAction();
	m_isClicked = true;
}

void QuestEntry::onMouseOver() {
	m_isMouseover = true;
}

bool QuestEntry::isClicked() const {
	return m_isClicked;
}

void QuestEntry::select() {
	m_isSelected = true;
}

GameObjectType QuestEntry::getConfiguredType() const {
	return _Interface;
}

void QuestEntry::deselect() {
	m_isSelected = false;
}

bool QuestEntry::isSelected() const {
	return m_isSelected;
}

///////////////////////////////////////////////////
//////////////// QUEST MARKER /////////////////////
///////////////////////////////////////////////////

LogQuestMarker::LogQuestMarker(const QuestData& questData, WorldInterface* interface) :
	QuestMarker(questData, interface->getScreen()->getCharacterCore()) {
	m_interface = interface;
	init();
}

void LogQuestMarker::onLeftClick() {
	if (!isActive()) {
		setActive(true);
		return;
	}
	jumpToQuest();
}

void LogQuestMarker::onRightClick() {
	if (!isActive()) return;
	setActive(false);
}

void LogQuestMarker::execSetActive() {
	if (isActive() && getCurrentStepData(m_questData, m_characterCore).empty()) {
		m_markerState = QuestMarkerState::Unreachable;
	}

	m_characterCore->setQuestTracked(m_questData.id, isActive());

	if (m_markerState == QuestMarkerState::Unreachable) {
		m_tooltipComponent->setTooltipText(g_textProvider->getText("LogQuestMarkerUnreachable"));
	}
	else if (isActive()) {
		m_tooltipComponent->setTooltipText(g_textProvider->getText("LogQuestMarkerActive"));
	}
	else {
		m_tooltipComponent->setTooltipText(g_textProvider->getText("LogQuestMarkerInactive"));
	}
}

void LogQuestMarker::init() {
	setActive(m_characterCore->isQuestTracked(m_questData.id));
}

void LogQuestMarker::jumpToQuest() {
	if (m_markerState == QuestMarkerState::Unreachable) return;

	auto markers = getCurrentStepData(m_questData, m_characterCore);
	m_interface->jumpToQuestMarker(m_questData.id, markers);
}
