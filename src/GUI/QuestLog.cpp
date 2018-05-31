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
	m_currentTab = QuestState::Started;
	m_entries = &m_startedQuests;

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

	// init tabbar
	const auto nTabs = 3;
	const auto width = nTabs * BUTTON_SIZE.x;
	const auto height = BUTTON_SIZE.y;
	const auto x = LEFT + 0.5f * (WIDTH - width);
	const auto y = TOP + GUIConstants::GUI_TABS_TOP;

	m_tabBar = new TabBar();
	m_tabBar->init(sf::FloatRect(x, y, width, height), nTabs);
	for (auto i = 0; i < nTabs; ++i) {
		m_tabBar->getTabButton(i)->setText(EnumNames::getQuestStateName(static_cast<QuestState>(i + 1)));
		m_tabBar->getTabButton(i)->setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	}

	// init scrolling
	m_scrollWindow = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_ORNAMENT_NONE), COLOR_WHITE, SCROLL_WINDOW_WIDTH, SCROLL_WINDOW_HEIGHT);
	m_scrollWindow.setPosition(sf::Vector2f(LEFT + SCROLL_WINDOW_LEFT, TOP + SCROLL_WINDOW_TOP));

	m_scrollBar = new ScrollBar(SCROLL_WINDOW_HEIGHT, m_window);
	m_scrollBar->setPosition(sf::Vector2f(LEFT + SCROLL_WINDOW_LEFT + SCROLL_WINDOW_WIDTH - ScrollBar::WIDTH, TOP + SCROLL_WINDOW_TOP));

	const sf::FloatRect scrollBox(LEFT + SCROLL_WINDOW_LEFT, TOP + SCROLL_WINDOW_TOP, SCROLL_WINDOW_WIDTH, SCROLL_WINDOW_HEIGHT);
	m_scrollHelper = new ScrollHelper(scrollBox);

	// init empty text
	m_emptyText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_emptyText.setString(g_textProvider->getText("NoQuests"));
	const auto bounds = m_emptyText.getBounds();
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
	m_selectedEntryId = -1;
}

void QuestLog::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	m_scrollBar->update(frameTime);

	updateSelection(frameTime);
	updateTabBar(frameTime);
	calculateEntryPositions();
	updateSelectableWindow();

	m_window->update(frameTime);
}

void QuestLog::updateTabBar(const sf::Time& frameTime) {
	m_tabBar->update(frameTime);
	auto const activeIndex = m_tabBar->getActiveTabIndex();
	auto const state = static_cast<QuestState>(activeIndex + 1);
	if (m_tabBar->getTabButton(activeIndex)->isClicked() && m_currentTab != state) {
		selectTab(state);
	}
}

void QuestLog::updateSelectableWindow() {
	if (!isWindowSelected()) {
		return;
	}

	if (g_inputController->isJustLeft()) {
		setLeftWindowSelected();
	}
}

void QuestLog::updateWindowSelected() {
	m_tabBar->setGamepadEnabled(isWindowSelected());
}

void QuestLog::execEntrySelected(const ScrollEntry* entry) {
	const auto questEntry = dynamic_cast<const QuestEntry*>(entry);
	showDescription(questEntry->getQuestID());
	m_selectedQuestID = questEntry->getQuestID();
}

bool QuestLog::isVisible() const {
	return m_isVisible;
}

void QuestLog::render(sf::RenderTarget& target) {
	if (!m_isVisible) return;

	m_window->render(target);
	target.draw(m_title);
	for (auto entry : (*m_entries)) {
		entry->render(m_scrollHelper->texture);
	}
	m_scrollHelper->render(target);

	m_tabBar->render(target);

	m_descriptionWindow->render(target);

	target.draw(m_scrollWindow);
	m_scrollBar->render(target);

	if ((*m_entries).empty()) {
		target.draw(m_emptyText);
	}
}

void QuestLog::renderAfterForeground(sf::RenderTarget& target) {
	for (auto entry : (*m_entries)) {
		entry->renderAfterForeground(target);
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
	if (m_selectedEntryId > -1) {
		(*m_entries)[m_selectedEntryId]->deselect();
		m_selectedEntryId = -1;
	}

	m_currentTab = state;
	m_entries = state == QuestState::Failed ?
		&m_failedQuests : state == QuestState::Completed ?
		&m_completedQuests : &m_startedQuests;

	selectEntry(0);

	m_scrollBar->setScrollPosition(0.f);
}

void QuestLog::reload() {
	m_scrollBar->scroll(0);

	clearAllEntries();
	std::vector<std::pair<QuestState, std::string>> mainQuests;
	std::vector<std::pair<QuestState, std::string>> nonMainQuests;

	for (auto& it : m_core->getData().questStates) {
		const QuestData* data = m_core->getQuestData(it.first);
		if (!data) continue;

		auto entries = it.second == QuestState::Failed ?
			&m_failedQuests : it.second == QuestState::Completed ?
			&m_completedQuests : &m_startedQuests;

		entries->push_back(new QuestEntry(*data, m_interface, it.second == QuestState::Started));
	}

	selectTab(m_currentTab);
}

void QuestLog::notifyJumpToQuest(const std::string& questId) {
	selectTab(QuestState::Started);
	for (auto i = 0; i < static_cast<int>(m_entries->size()); ++i)
	{
		if (dynamic_cast<QuestEntry*>((*m_entries)[i])->getQuestID() == questId) {
			selectEntry(i);
			break;
		}
	}
}

bool QuestLog::isEntryInvisible(const ScrollEntry* entry) const {
	const auto pos = entry->getPosition();
	return pos.y < TOP + SCROLL_WINDOW_TOP ||
		pos.y + GUIConstants::CHARACTER_SIZE_M > TOP + SCROLL_WINDOW_TOP + SCROLL_WINDOW_HEIGHT;
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
	updateColor();
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

	ScrollEntry::update(frameTime);
}

void QuestEntry::updateColor() {
	m_name.setColor(isSelected() ? COLOR_WHITE : sf::Color(180, 180, 180));
}

const std::string& QuestEntry::getQuestID() const {
	return m_data.id;
}

void QuestEntry::setPosition(const sf::Vector2f& pos) {
	ScrollEntry::setPosition(pos);
	m_name.setPosition(pos);
	if (m_questMarker) {
		m_questMarker->setPosition(pos + sf::Vector2f(0.f, -0.5f * (QuestMarker::SIZE - GUIConstants::CHARACTER_SIZE_M)));
	}
}

void QuestEntry::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_name);
	if (m_questMarker) {
		m_questMarker->render(renderTarget);
	}
}

void QuestEntry::renderAfterForeground(sf::RenderTarget& renderTarget) {
	ScrollEntry::renderAfterForeground(renderTarget);
	if (m_questMarker) {
		m_questMarker->renderAfterForeground(renderTarget);
	}
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
	g_inputController->lockAction();
	if (!isActive()) {
		setActive(true);
		return;
	}
	jumpToQuest();
}

void LogQuestMarker::onRightClick() {
	g_inputController->lockAction();
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
