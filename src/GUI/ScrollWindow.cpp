#include "GUI/ScrollWindow.h"
#include "GUI/ScrollBar.h"
#include "GUI/ScrollHelper.h"

void ScrollWindow::selectEntry(int entryId) {
	if (entryId < 0 || entryId > static_cast<int>((*m_entries).size()) - 1) {
		return;
	}

	if (entryId == m_selectedEntryId) {
		return;
	}

	if (m_selectedEntryId > -1) {
		(*m_entries)[m_selectedEntryId]->deselect();
	}

	m_selectedEntryId = entryId;
	auto newEntry = (*m_entries)[m_selectedEntryId];
	newEntry->select();

	execEntrySelected(newEntry);
}

void ScrollWindow::calculateEntryPositions() {
	const auto rows = static_cast<int>((*m_entries).size());
	const auto steps = rows - getEntryCount() + 1;

	m_scrollBar->setDiscreteSteps(steps);

	const auto scrollPos = m_scrollBar->getDiscreteScrollPosition();

	if (2.f * scrollPos * GUIConstants::CHARACTER_SIZE_M != m_scrollHelper->nextOffset) {
		m_scrollHelper->lastOffset = m_scrollHelper->nextOffset;
		m_scrollHelper->nextOffset = 2.f * scrollPos * GUIConstants::CHARACTER_SIZE_M;
	}

	const auto animationTime = 0.1f;
	const auto time = m_scrollBar->getScrollTime().asSeconds();
	if (time >= animationTime) {
		m_scrollHelper->lastOffset = m_scrollHelper->nextOffset;
	}
	const auto start = m_scrollHelper->lastOffset;
	const auto change = m_scrollHelper->nextOffset - m_scrollHelper->lastOffset;
	const auto effectiveScrollOffset = easeInOutQuad(time, start, change, animationTime);

	const auto xOffset = getLeft() + 2 * getWindowMargin();
	auto yOffset = getTop() + getWindowMargin() + GUIConstants::CHARACTER_SIZE_M - effectiveScrollOffset;

	for (auto entry : (*m_entries)) {
		entry->setBoundingBox(sf::FloatRect(xOffset, yOffset + 0.5f * GUIConstants::CHARACTER_SIZE_M, getWidth() - ScrollBar::WIDTH, 2.f * GUIConstants::CHARACTER_SIZE_M));
		entry->setPosition(sf::Vector2f(xOffset, yOffset));
		yOffset += 2.f * GUIConstants::CHARACTER_SIZE_M;
	}
}

void ScrollWindow::updateSelection(const sf::Time& frameTime) {
	if (g_inputController->isJustDown()) {
		selectEntry(m_selectedEntryId + 1);
		if (isEntryInvisible((*m_entries)[m_selectedEntryId])) {
			m_scrollBar->scroll(1);
		}
		m_downActiveTime = frameTime;
		return;
	}

	if (g_inputController->isJustUp()) {
		selectEntry(m_selectedEntryId - 1);
		if (isEntryInvisible((*m_entries)[m_selectedEntryId])) {
			m_scrollBar->scroll(-1);
		}
		m_upActiveTime = frameTime;
		return;
	}

	if (m_upActiveTime > sf::Time::Zero) {
		if (g_inputController->isUp()) {
			m_upActiveTime += frameTime;
		}
		else {
			m_upActiveTime = sf::Time::Zero;
			return;
		}
	}

	if (m_downActiveTime > sf::Time::Zero) {
		if (g_inputController->isDown()) {
			m_downActiveTime += frameTime;
		}
		else {
			m_downActiveTime = sf::Time::Zero;
			return;
		}
	}

	m_timeSinceTick += frameTime;
	if (m_timeSinceTick < SCROLL_TICK_TIME) return;

	if (m_upActiveTime > SCROLL_TIMEOUT) {
		selectEntry(std::max(m_selectedEntryId - 1, 0));
		m_timeSinceTick = sf::Time::Zero;
		if (isEntryInvisible((*m_entries)[m_selectedEntryId])) {
			m_scrollBar->scroll(-1);
		}
		return;
	}

	if (m_downActiveTime > SCROLL_TIMEOUT) {
		selectEntry(std::min(m_selectedEntryId + 1, static_cast<int>((*m_entries).size()) - 1));
		m_timeSinceTick = sf::Time::Zero;
		if (isEntryInvisible((*m_entries)[m_selectedEntryId])) {
			m_scrollBar->scroll(1);
		}
		return;
	}

	for (auto i = 0; i < static_cast<int>((*m_entries).size()); ++i) {
		auto entry = (*m_entries)[i];
		if (isEntryInvisible(entry)) continue;
		entry->update(frameTime);
		if (entry->isClicked()) {
			selectEntry(i);
			return;
		}
	}
}

// <<< ScrollEntry >>>

void ScrollEntry::update(const sf::Time& frameTime) {
	m_isMouseover = false;
	m_isClicked = false;
	GameObject::update(frameTime);
	updateColor();
}

void ScrollEntry::onLeftJustPressed() {
	g_inputController->lockAction();
	m_isClicked = true;
}

void ScrollEntry::onMouseOver() {
	m_isMouseover = true;
}

bool ScrollEntry::isClicked() const {
	return m_isClicked;
}

void ScrollEntry::select() {
	m_isSelected = true;
	updateColor();
}

void ScrollEntry::deselect() {
	m_isSelected = false;
	updateColor();
}

bool ScrollEntry::isSelected() const {
	return m_isSelected;
}

bool ScrollEntry::isMouseover() const {
	return m_isMouseover;
}

GameObjectType ScrollEntry::getConfiguredType() const {
	return _Interface;
}
