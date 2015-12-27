#include "GUI/CookingWindow.h"
#include "CharacterCore.h"
#include "Screens/MapScreen.h"

using namespace std;

const sf::Vector2f TEXT_OFFSET = sf::Vector2f(255.f, 10.f);
const sf::FloatRect BOX = sf::FloatRect(0.f, WINDOW_HEIGHT - 200.f, WINDOW_WIDTH, 200.f);
const int CHAR_SIZE_TITLE = 16;
const int CHAR_SIZE_OPTIONS = 12;

CookingWindow::CookingWindow(MapScreen* screen) : Window(BOX, WindowOrnamentStyle::LARGE, sf::Color(0, 0, 0, 100), sf::Color(0, 0, 0, 100), sf::Color::White) {
	m_screen = screen;
	m_cookingSprite = sf::Sprite(*(g_resourceManager->getTexture(ResourceID::Texture_misc_cooking)));
	m_title.setString(g_textProvider->getText("Fireplace"));
	m_title.setCharacterSize(CHAR_SIZE_TITLE);
	m_title.setColor(CENDRIC_COLOR_LIGHT_PURPLE);
	
	setPosition(getPosition());
	reload();
}

void CookingWindow::setPosition(const sf::Vector2f& pos) {
	Window::setPosition(pos); 
	m_cookingSprite.setPosition(sf::Vector2f(pos.x, WINDOW_HEIGHT - 250.f));
	m_title.setPosition(sf::Vector2f(pos.x + TEXT_OFFSET.x, pos.y + TEXT_OFFSET.y));
}

CookingWindow::~CookingWindow() {
	g_resourceManager->deleteResource(ResourceID::Texture_misc_cooking);
}

void CookingWindow::reload() {
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
	bool chooseOption = false;

	int oldOption = m_chosenOption;
	if (g_inputController->isKeyJustPressed(Key::Up)) {
		m_chosenOption = std::max(m_chosenOption - 1, 0);
	}
	else if (g_inputController->isKeyJustPressed(Key::Down)) {
		m_chosenOption = std::min(m_chosenOption + 1, static_cast<int>(m_options.size()) - 1);
	}
	for (int i = 0; i < m_options.size(); i++) {
		m_options[i].update(frameTime);
		if (m_options[i].isClicked()) {
			if (i == m_chosenOption) {
				// a chosen option was clicked again
				chooseOption = true;
			}
			m_chosenOption = i;
		}
	}

	if (oldOption != m_chosenOption) {
		m_options[oldOption].deselect();
		m_options[m_chosenOption].select();
	}

	if (chooseOption || g_inputController->isKeyJustPressed(Key::Confirm) || g_inputController->isKeyJustPressed(Key::Interact)) {
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

void CookingWindow::cookItem(const std::string& itemID) {
	auto& it = m_screen->getCharacterCore()->getItems()->find(itemID);
	if (it == m_screen->getCharacterCore()->getItems()->end()) {
		g_logger->logError("CookingWindow", "Item to cook not found in character core!");
		return;
	}
	ItemFoodBean food = g_databaseManager->getItemFoodBean(itemID);
	if (food.status == BeanStatus::NotSet || !food.is_cookable || food.cooked_item_id.empty()) {
		g_logger->logError("CookingWindow", "Cannot cook item with id " + itemID);
		return;
	}
	m_screen->notifyItemChange(itemID, -1);
	m_screen->notifyItemChange(food.cooked_item_id, 1);
}

void CookingWindow::render(sf::RenderTarget& renderTarget) {
	Window::render(renderTarget);
	renderTarget.draw(m_title);

	for (auto& it : m_options) {
		it.render(renderTarget);
	}

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
		m_text.setString(text + g_textProvider->getText("Cancel"));
	}
	else {
		std::string textString = g_textProvider->getText(cookedItemID, "item");
		textString += " (" + g_textProvider->getText(itemID, "item") + " " + std::to_string(count)  + ")";
		m_text.setString(textString);
	}
	m_text.setCharacterSize(CHAR_SIZE_OPTIONS);
	m_text.setColor(sf::Color::White);
	setBoundingBox(sf::FloatRect(0.f, 0.f, m_text.getLocalBounds().width, 20.f));
	setPosition(sf::Vector2f(TEXT_OFFSET.x, BOX.top + TEXT_OFFSET.y + 30.f + (nr * 20.f)));
	setInputInDefaultView(true);
}

void CookingOption::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_text.setPosition(pos);
}

void CookingOption::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_text);
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
	m_text.setColor(sf::Color::White);
	m_isSelected = true;
}

GameObjectType CookingOption::getConfiguredType() const {
	return GameObjectType::_Undefined;
}

void CookingOption::deselect() {
	m_text.setColor(CENDRIC_COLOR_GREY);
	m_isSelected = false;
}

bool CookingOption::isSelected() const {
	return m_isSelected;
}

const std::string& CookingOption::getItemID() const {
	return m_itemID;
}

