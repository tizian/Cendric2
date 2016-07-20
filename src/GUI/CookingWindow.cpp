#include "GUI/CookingWindow.h"
#include "GUI/ScrollBar.h"
#include "GUI/ScrollHelper.h"
#include "GUI/SlicedSprite.h"
#include "CharacterCore.h"
#include "Screens/MapScreen.h"
#include "GlobalResource.h"

using namespace std;

const float CookingWindow::LEFT_OFFSET = 270.f;	// offset to have space for sprite
const float CookingWindow::RIGHT_OFFSET = 100.f;

const float CookingWindow::TEXT_WIDTH = WINDOW_WIDTH - LEFT_OFFSET - RIGHT_OFFSET;

const int CookingWindow::OPTION_COUNT = 5;

const float CookingWindow::WINDOW_MARGIN = 6.f;

const float CookingWindow::WIDTH = WINDOW_WIDTH;
const float CookingWindow::HEIGHT = 200.f;
const float CookingWindow::TOP = WINDOW_HEIGHT - HEIGHT;
const float CookingWindow::LEFT = 0.f;

const float CookingWindow::SCROLL_WINDOW_TOP = 2.f * WINDOW_MARGIN + GUIConstants::CHARACTER_SIZE_L + GUIConstants::TEXT_OFFSET;
const float CookingWindow::SCROLL_WINDOW_LEFT = LEFT_OFFSET;
const float CookingWindow::SCROLL_WINDOW_WIDTH = TEXT_WIDTH;
const float CookingWindow::SCROLL_WINDOW_HEIGHT = 4 * WINDOW_MARGIN + OPTION_COUNT * GUIConstants::CHARACTER_SIZE_M + (OPTION_COUNT - 1) * GUIConstants::CHARACTER_SIZE_M;

CookingWindow::CookingWindow(MapScreen* screen) : Window(sf::FloatRect(LEFT, TOP, WIDTH, HEIGHT) , GUIOrnamentStyle::LARGE, sf::Color(0, 0, 0, 200), COLOR_WHITE) {
	m_screen = screen;
	m_cookingSprite = sf::Sprite(*(g_resourceManager->getTexture(GlobalResource::TEX_COOKING)));
	m_title.setString(g_textProvider->getText("Fireplace"));
	m_title.setCharacterSize(GUIConstants::CHARACTER_SIZE_L);
	m_title.setColor(COLOR_LIGHT_PURPLE);
	
	setPosition(getPosition());

	m_scrollWindow = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_ORNAMENT_NONE), COLOR_WHITE, SCROLL_WINDOW_WIDTH, SCROLL_WINDOW_HEIGHT);
	m_scrollWindow.setPosition(sf::Vector2f(LEFT + SCROLL_WINDOW_LEFT, TOP + SCROLL_WINDOW_TOP));

	m_scrollBar = new ScrollBar(SCROLL_WINDOW_HEIGHT);
	m_scrollBar->setPosition(sf::Vector2f(LEFT + SCROLL_WINDOW_LEFT + SCROLL_WINDOW_WIDTH - ScrollBar::WIDTH, TOP + SCROLL_WINDOW_TOP));

	sf::FloatRect scrollBox(LEFT + SCROLL_WINDOW_LEFT, TOP + SCROLL_WINDOW_TOP, SCROLL_WINDOW_WIDTH, SCROLL_WINDOW_HEIGHT);
	m_scrollHelper = new ScrollHelper(scrollBox);

	reload();
}

void CookingWindow::setPosition(const sf::Vector2f& pos) {
	Window::setPosition(pos); 
	m_cookingSprite.setPosition(sf::Vector2f(pos.x, WINDOW_HEIGHT - 250.f));
	m_title.setPosition(sf::Vector2f(pos.x + LEFT_OFFSET, pos.y + 3 * WINDOW_MARGIN));
}

CookingWindow::~CookingWindow() {
}

void CookingWindow::reload() {
	m_scrollBar->setScrollPosition(0.f);
	m_options.clear();

	int nr = 0;
	for (auto& item : *m_screen->getCharacterCore()->getItems()) {
		ItemFoodBean food = g_databaseManager->getItemFoodBean(item.first);
		if (food.status == BeanStatus::Filled && food.is_cookable) {
			CookingOption option(item.first, food.cooked_item_id, item.second, nr);
			option.deselect();
			m_options.push_back(option);
			nr++;
		}
	}

	CookingOption cancelOption("", "", -1, nr);
	cancelOption.deselect();
	m_options.push_back(cancelOption);

	m_chosenOption = 0;
	m_options[m_chosenOption].select();
}

bool CookingWindow::updateWindow(const sf::Time frameTime) {
	if (g_inputController->isKeyJustPressed(Key::Escape)) {
		g_inputController->lockAction();
		return false;
	}

	bool chooseOption = false;

	m_scrollBar->update(frameTime);

	int oldOption = m_chosenOption;

	updateScrolling(frameTime);

	for (size_t i = 0; i < m_options.size(); ++i) {
		if (i == static_cast<size_t>(m_chosenOption)) {
			m_options[i].setColor(COLOR_WHITE);
		}
		else if (g_inputController->isMouseOver(m_options[i].getBoundingBox(), true)) {
			m_options[i].setColor(COLOR_LIGHT_PURPLE);
		}
		else {
			m_options[i].setColor(COLOR_GREY);
		}
	}

	for (size_t i = 0; i < m_options.size(); i++) {
		sf::Vector2f pos = m_options[i].getPosition();
		if (pos.y < TOP || pos.y + GUIConstants::CHARACTER_SIZE_M > TOP + HEIGHT) continue;
		m_options[i].update(frameTime);
		if (m_options[i].isClicked()) {
			if (static_cast<int>(i) == m_chosenOption) {
				// a chosen option was clicked again
				chooseOption = true;
			}
			m_chosenOption = static_cast<int>(i);
		}
	}

	if (oldOption != m_chosenOption) {
		m_options[oldOption].deselect();
		m_options[m_chosenOption].select();
		g_resourceManager->playSound(m_sound, GlobalResource::SOUND_GUI_MENUCURSOR, true);
	}

	calculateEntryPositions();

	if (chooseOption || g_inputController->isSelected()) {
		g_inputController->lockAction();
		const std::string& itemID = m_options[m_chosenOption].getItemID();
		if (itemID.empty()) {
			// end cooking window
			return false;
		}
		
		cookItem(itemID);
		reload();
	}

	return true;
}

void CookingWindow::calculateEntryPositions() {
	int rows = static_cast<int>(m_options.size());
	int steps = rows - OPTION_COUNT + 1;

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
	float yOffset = TOP + SCROLL_WINDOW_TOP + 2 * WINDOW_MARGIN - effectiveScrollOffset;

	for (auto& it : m_options) {
		it.setBoundingBox(sf::FloatRect(xOffset, yOffset + 0.5f * GUIConstants::CHARACTER_SIZE_M, SCROLL_WINDOW_WIDTH - ScrollBar::WIDTH, 2.f * GUIConstants::CHARACTER_SIZE_M));
		it.setPosition(sf::Vector2f(xOffset, yOffset));
		yOffset += 2.f * GUIConstants::CHARACTER_SIZE_M;
	}
}

void CookingWindow::updateScrolling(const sf::Time& frameTime) {
	if (g_inputController->isKeyJustPressed(Key::Up)) {
		m_chosenOption = std::max(m_chosenOption - 1, 0);
		CookingOption& option = m_options[m_chosenOption];
		if (option.getPosition().y < TOP + SCROLL_WINDOW_TOP) {
			m_scrollBar->scroll(-1);
		}
		m_upActiveTime = frameTime;
		return;
	}

	if (g_inputController->isKeyJustPressed(Key::Down)) {
		m_chosenOption = std::min(m_chosenOption + 1, static_cast<int>(m_options.size()) - 1);
		CookingOption& option = m_options[m_chosenOption];
		if (option.getPosition().y + option.getSize().y > TOP + SCROLL_WINDOW_TOP + SCROLL_WINDOW_HEIGHT) {
			m_scrollBar->scroll(1);
		}
		m_downActiveTime = frameTime;
		return;
	}

	if (m_upActiveTime > sf::Time::Zero) {
		if (g_inputController->isKeyActive(Key::Up)) {
			m_upActiveTime += frameTime;
		}
		else {
			m_upActiveTime = sf::Time::Zero;
			return;
		}
	}

	if (m_downActiveTime > sf::Time::Zero) {
		if (g_inputController->isKeyActive(Key::Down)) {
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
		m_chosenOption = std::max(m_chosenOption - 1, 0);
		CookingOption& option = m_options[m_chosenOption];
		m_timeSinceTick = sf::Time::Zero;
		if (option.getPosition().y < TOP) {
			m_scrollBar->scroll(-1);
		}
		return;
	}

	if (m_downActiveTime > SCROLL_TIMEOUT) {
		m_chosenOption = std::min(m_chosenOption + 1, static_cast<int>(m_options.size()) - 1);
		CookingOption& option = m_options[m_chosenOption];
		m_timeSinceTick = sf::Time::Zero;
		if (option.getPosition().y + option.getSize().y > TOP + HEIGHT) {
			m_scrollBar->scroll(1);
		}
		return;
	}
}

void CookingWindow::cookItem(const std::string& itemID) {
	const auto& it = m_screen->getCharacterCore()->getItems()->find(itemID);
	if (it == m_screen->getCharacterCore()->getItems()->end()) {
		g_logger->logError("CookingWindow", "Item to cook not found in character core!");
		return;
	}
	ItemFoodBean food = g_databaseManager->getItemFoodBean(itemID);
	if (food.status == BeanStatus::NotSet || !food.is_cookable || food.cooked_item_id.empty()) {
		g_logger->logError("CookingWindow", "Cannot cook item with id " + itemID);
		return;
	}
	m_screen->notifyItemConversion(itemID, food.cooked_item_id, 1);
}

void CookingWindow::render(sf::RenderTarget& renderTarget) {
	Window::render(renderTarget);
	renderTarget.draw(m_title);

	for (auto& it : m_options) {
		it.render(m_scrollHelper->texture);
	}
	m_scrollHelper->render(renderTarget);

	renderTarget.draw(m_scrollWindow);
	m_scrollBar->render(renderTarget);

	renderTarget.draw(m_cookingSprite);
}

// Cooking Option

CookingOption::CookingOption(const std::string& itemID, const std::string& cookedItemID, int count, int nr) {
	m_itemID = itemID;
	// if the itemID is empty, this is a cancel option
	if (itemID.empty()) {
		std::string text = "";
		if (nr == 0) {
			text += g_textProvider->getText("NothingToCook") + " ";
		}
		m_text.setString(text + g_textProvider->getText("CancelCooking"));
	}
	else {
		std::string textString = g_textProvider->getText(cookedItemID, "item");
		textString += " (" + g_textProvider->getText(itemID, "item") + " " + std::to_string(count)  + ")";
		m_text.setString(textString);
	}
	m_text.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_text.setColor(COLOR_WHITE);
	setBoundingBox(sf::FloatRect(0.f, 0.f, m_text.getLocalBounds().width, 20.f));
	setInputInDefaultView(true);
}

void CookingOption::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_text.setPosition(pos);
}

void CookingOption::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_text);
}

void CookingOption::setColor(const sf::Color& color) {
	m_text.setColor(color);
}

void CookingOption::onLeftClick() {
	m_isClicked = true;
}

bool CookingOption::isClicked() {
	bool wasClicked = m_isClicked;
	m_isClicked = false;
	return wasClicked;
}

void CookingOption::select() {
	m_text.setColor(COLOR_WHITE);
	m_isSelected = true;
}

GameObjectType CookingOption::getConfiguredType() const {
	return GameObjectType::_Undefined;
}

void CookingOption::deselect() {
	m_text.setColor(COLOR_GREY);
	m_isSelected = false;
}

bool CookingOption::isSelected() const {
	return m_isSelected;
}

const std::string& CookingOption::getItemID() const {
	return m_itemID;
}

