#include "GUI/DialogueWindow.h"
#include "GUI/ScrollBar.h"
#include "GUI/ScrollHelper.h"
#include "GUI/SlicedSprite.h"
#include "CharacterCore.h"
#include "Map/NPC.h"
#include "GlobalResource.h"

const std::string CENDRIC_NAME = "Cendric";

const float DialogueWindow::LEFT_OFFSET = 270.f;	// offset to have space for sprite
const float DialogueWindow::RIGHT_OFFSET = 100.f;

const float DialogueWindow::TEXT_WIDTH = WINDOW_WIDTH - LEFT_OFFSET - RIGHT_OFFSET;

const int DialogueWindow::OPTION_COUNT = 5;

const float DialogueWindow::WINDOW_MARGIN = 6.f;

const float DialogueWindow::WIDTH = WINDOW_WIDTH;
const float DialogueWindow::HEIGHT = 200.f;
const float DialogueWindow::TOP = WINDOW_HEIGHT - HEIGHT;
const float DialogueWindow::LEFT = 0.f;

const float DialogueWindow::SCROLL_WINDOW_TOP = 2.f * WINDOW_MARGIN + GUIConstants::CHARACTER_SIZE_L + GUIConstants::TEXT_OFFSET;
const float DialogueWindow::SCROLL_WINDOW_LEFT = LEFT_OFFSET;
const float DialogueWindow::SCROLL_WINDOW_WIDTH = TEXT_WIDTH;
const float DialogueWindow::SCROLL_WINDOW_HEIGHT = 4 * WINDOW_MARGIN + OPTION_COUNT * GUIConstants::CHARACTER_SIZE_M + (OPTION_COUNT - 1) * GUIConstants::CHARACTER_SIZE_M;

DialogueWindow::DialogueWindow() : Window(sf::FloatRect(LEFT, TOP, WIDTH, HEIGHT), GUIOrnamentStyle::LARGE, sf::Color(0, 0, 0, 200), COLOR_WHITE) {
	m_speakerSprite.setTextureRect(sf::IntRect(0, 0, 250, 250));

	m_speakerText = new BitmapText("");
	m_speakerText->setCharacterSize(GUIConstants::CHARACTER_SIZE_L);
	m_speakerText->setColor(COLOR_LIGHT_PURPLE);

	m_dialogueText = new sf::Text();
	m_dialogueText->setFont(*g_resourceManager->getFont(GlobalResource::FONT_TTF_DIALOGUE));
	m_dialogueText->setCharacterSize(GUIConstants::CHARACTER_SIZE_DIALOGUE);
	m_dialogueText->setFillColor(COLOR_WHITE);

	setPosition(sf::Vector2f(LEFT, TOP));

	m_scrollWindow = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_ORNAMENT_NONE), COLOR_WHITE, SCROLL_WINDOW_WIDTH, SCROLL_WINDOW_HEIGHT);
	m_scrollWindow.setPosition(sf::Vector2f(LEFT + SCROLL_WINDOW_LEFT, TOP + SCROLL_WINDOW_TOP));

	m_scrollBar = new ScrollBar(SCROLL_WINDOW_HEIGHT);
	m_scrollBar->setPosition(sf::Vector2f(LEFT + SCROLL_WINDOW_LEFT + SCROLL_WINDOW_WIDTH - ScrollBar::WIDTH, TOP + SCROLL_WINDOW_TOP));

	sf::FloatRect scrollBox(LEFT + SCROLL_WINDOW_LEFT, TOP + SCROLL_WINDOW_TOP, SCROLL_WINDOW_WIDTH, SCROLL_WINDOW_HEIGHT);
	m_scrollHelper = new ScrollHelper(scrollBox);
}

void DialogueWindow::setPosition(const sf::Vector2f& pos) {
	Window::setPosition(pos);
	m_speakerSprite.setPosition(sf::Vector2f(pos.x, pos.y - 250.f + HEIGHT));
	m_speakerText->setPosition(sf::Vector2f(pos.x + LEFT_OFFSET, pos.y + 3 * WINDOW_MARGIN));
	m_dialogueText->setPosition(sf::Vector2f(pos.x + LEFT_OFFSET, pos.y + SCROLL_WINDOW_TOP));
}

DialogueWindow::~DialogueWindow() {
	delete m_merchantInterface;
	delete m_dialogueText;
	delete m_speakerText;
	delete m_scrollBar;
	delete m_scrollHelper;
	delete m_dialogue;
}

void DialogueWindow::load(NPC* npc, WorldScreen* screen) {
	g_resourceManager->loadTexture(npc->getNPCData().dialoguetexture, ResourceType::Map);

	setNPC(npc);
	setDialogue(npc->getNPCData().dialogueID, screen);
}

NPC* DialogueWindow::getNPC() {
	return m_npc;
}

void DialogueWindow::setDialogue(const std::string& dialogueID, WorldScreen* screen) {
	delete m_dialogue;
	m_dialogue = new Dialogue();
	m_dialogue->reload(dialogueID, screen, this);
	m_screen = screen;

	m_dialogue->updateWindow();
	m_dialogueTimeout = DIALOGUE_TIMEOUT;
}

void DialogueWindow::setNPC(NPC* npc) {
	m_npc = npc;
	m_npcID = m_npc->getNPCData().id;
	m_dialogueTextID = m_npc->getNPCData().textType;
	m_npcName = g_textProvider->getText(removeDigits(m_npcID), "npc");
}

void DialogueWindow::setNPCTalking(const std::string& text) {
	m_options.clear();
	m_speakerSprite.setTexture(*g_resourceManager->getTexture(m_npc->getNPCData().dialoguetexture));
	m_speakerText->setString(m_npcName);

	std::string line = g_textProvider->getCroppedText(text, m_dialogueTextID, GUIConstants::CHARACTER_SIZE_M, static_cast<int>(TEXT_WIDTH), true);
	std::basic_string<sf::Uint32> utf32line;
	sf::Utf8::toUtf32(line.begin(), line.end(), std::back_inserter(utf32line));
	m_dialogueText->setString(utf32line);
}

void DialogueWindow::setCendricTalking(const std::string& text) {
	m_options.clear();
	m_speakerSprite.setTexture(*getCendricTexture());
	m_speakerText->setString(CENDRIC_NAME);

	std::string line = g_textProvider->getCroppedText(text, m_dialogueTextID, GUIConstants::CHARACTER_SIZE_M, static_cast<int>(TEXT_WIDTH), true);
	std::basic_string<sf::Uint32> utf32line;
	sf::Utf8::toUtf32(line.begin(), line.end(), std::back_inserter(utf32line));
	m_dialogueText->setString(utf32line);
}

void DialogueWindow::setNPCTrading(const std::string& text) {
	m_options.clear();

	delete m_merchantInterface;
	WorldScreen* worldScreen = dynamic_cast<WorldScreen*>(m_screen);
	m_merchantInterface = new MerchantInterface(worldScreen, m_npcID);
	worldScreen->getProgressLog()->setVisible(false);
	setPosition(sf::Vector2f(LEFT, TOP + HEIGHT + 50.f));
}

void DialogueWindow::setDialogueChoice(const std::vector<std::pair<ChoiceTranslation, int>>& choices) {
	m_scrollBar->setScrollPosition(0.f);
	m_options.clear();
	m_dialogueText->setString("");
	m_speakerSprite.setTexture(*getCendricTexture());
	m_speakerText->setString(CENDRIC_NAME);

	for (size_t i = 0; i < choices.size(); ++i) {
		DialogueOptionType optionType = DialogueOptionType::Default;
		if (choices[i].second == -1) {
			optionType = DialogueOptionType::End;
		}
		else if (m_dialogue->isTradeNode(choices[i].second)) {
			optionType = DialogueOptionType::Trade;
		}
		DialogueOption option(choices[i].first, m_dialogueTextID, optionType, m_screen);
		option.deselect();
		m_options.push_back(option);
	}

	m_chosenOption = 0;
	m_options[m_chosenOption].select();
}

bool DialogueWindow::updateDialogue(const sf::Time frameTime) {
	updateTime(m_dialogueTimeout, frameTime);

	if (g_inputController->isKeyJustPressed(Key::Escape)) {
		// check if we can leave the window
		if (m_dialogue->isEndable()) {
			g_inputController->lockAction();
			return false;
		}
	}

	bool chooseOption = false;

	if (m_merchantInterface != nullptr) {
		m_merchantInterface->update(frameTime);
		if (m_merchantInterface->isCancelled()) {
			delete m_merchantInterface;
			m_merchantInterface = nullptr;
			dynamic_cast<WorldScreen*>(m_screen)->getProgressLog()->setVisible(true);
			setPosition(sf::Vector2f(LEFT, TOP));
			m_dialogue->setNextNode(-1);
			return m_dialogue->updateWindow();
		}
		return true;
	}

	if (!m_options.empty()) {

		m_scrollBar->update(frameTime);

		int oldOption = m_chosenOption;

		updateScrolling(frameTime);

		// update mouseover colors.
		for (size_t i = 0; i < m_options.size(); ++i) {
			if (!m_options[i].isSelectable()) continue;
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

		for (size_t i = 0; i < m_options.size(); ++i) {
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
			g_resourceManager->playSound(GlobalResource::SOUND_GUI_MENUCURSOR);
		}

		calculateEntryPositions();

	}

	if (m_options.empty() && ((m_dialogueTimeout == sf::Time::Zero && g_inputController->isMouseClickedLeft()) || g_inputController->isMouseClickedRight())) {
		m_dialogue->setNextNode(-1);
		return m_dialogue->updateWindow();
	}

	if ((m_dialogueTimeout == sf::Time::Zero && chooseOption) || g_inputController->isSelected()) {
		if (m_options.empty()) {
			m_dialogue->setNextNode(-1);
		}
		else {
			if (!m_options[m_chosenOption].executeCrafting()) {
				m_screen->setNegativeTooltip("NotEnoughMaterials");
				return true;
			}
			m_dialogue->setNextNode(m_chosenOption);
		}
		m_dialogueTimeout = DIALOGUE_TIMEOUT;
		g_inputController->lockAction();
		return m_dialogue->updateWindow();
	}
	return true;
}

void DialogueWindow::calculateEntryPositions() {
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
	float yOffset = TOP + SCROLL_WINDOW_TOP + 2 * WINDOW_MARGIN - effectiveScrollOffset - 0.5f * GUIConstants::CHARACTER_SIZE_M;

	for (auto& it : m_options) {
		it.setBoundingBox(sf::FloatRect(xOffset, yOffset + 0.5f * GUIConstants::CHARACTER_SIZE_M, SCROLL_WINDOW_WIDTH - ScrollBar::WIDTH, 2.f * GUIConstants::CHARACTER_SIZE_M));
		it.setPosition(sf::Vector2f(xOffset, yOffset));
		yOffset += 2.f * GUIConstants::CHARACTER_SIZE_M;
	}
}

void DialogueWindow::updateScrolling(const sf::Time& frameTime) {
	if (g_inputController->isJustUp()) {
		m_chosenOption = std::max(m_chosenOption - 1, 0);
		DialogueOption& option = m_options[m_chosenOption];
		if (option.getPosition().y < TOP + SCROLL_WINDOW_TOP) {
			m_scrollBar->scroll(-1);
		}
		m_upActiveTime = frameTime;
		return;
	}

	if (g_inputController->isJustDown()) {
		m_chosenOption = std::min(m_chosenOption + 1, static_cast<int>(m_options.size()) - 1);
		DialogueOption& option = m_options[m_chosenOption];
		if (option.getPosition().y + option.getSize().y > TOP + SCROLL_WINDOW_TOP + SCROLL_WINDOW_HEIGHT) {
			m_scrollBar->scroll(1);
		}
		m_downActiveTime = frameTime;
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
		m_chosenOption = std::max(m_chosenOption - 1, 0);
		DialogueOption& option = m_options[m_chosenOption];
		m_timeSinceTick = sf::Time::Zero;
		if (option.getPosition().y < TOP + SCROLL_WINDOW_TOP) {
			m_scrollBar->scroll(-1);
		}
		return;
	}

	if (m_downActiveTime > SCROLL_TIMEOUT) {
		m_chosenOption = std::min(m_chosenOption + 1, static_cast<int>(m_options.size()) - 1);
		DialogueOption& option = m_options[m_chosenOption];
		m_timeSinceTick = sf::Time::Zero;
		if (option.getPosition().y + option.getSize().y > TOP + SCROLL_WINDOW_TOP + SCROLL_WINDOW_HEIGHT) {
			m_scrollBar->scroll(1);
		}
		return;
	}
}

void DialogueWindow::render(sf::RenderTarget& renderTarget) {
	Window::render(renderTarget);

	renderTarget.draw(*m_speakerText);
	if (!m_options.empty()) {
		for (auto& it : m_options) {
			it.render(m_scrollHelper->texture);
		}
		m_scrollHelper->render(renderTarget);

		renderTarget.draw(m_scrollWindow);
		m_scrollBar->render(renderTarget);
	}
	else {
		renderTarget.draw(*m_dialogueText);
	}

	if (m_merchantInterface != nullptr) {
		m_merchantInterface->render(renderTarget);
		m_merchantInterface->renderAfterForeground(renderTarget);
	}

	renderTarget.draw(m_speakerSprite);
}


sf::Texture* DialogueWindow::getCendricTexture() {
	if (!m_screen->getCharacterCore()->isConditionFulfilled("boss", "BossVelius")) {
		return g_resourceManager->getTexture(GlobalResource::TEX_DIALOGUE);
	}
	return g_resourceManager->getTexture(GlobalResource::TEX_DIALOGUE_END);
}

// Dialogue Option

DialogueOption::DialogueOption(const ChoiceTranslation& trans, const std::string& dialogueID, DialogueOptionType type, Screen* screen) {
	m_screen = screen;
	m_translation = trans;
	std::string textString;
	if (trans.crafting.item.empty()) {
		textString = g_textProvider->getText(trans.text, dialogueID, true, true);
		if (!trans.item.first.empty()) {
			textString.append(" (" + std::to_string(trans.item.second) + " " + g_textProvider->getText(trans.item.first, "item", false, true) + ")");
		}
	}
	else {
		// crafting item, format the text and check whether the items are available.
		textString.append(g_textProvider->getText(trans.crafting.item, "item", false, true) + " (");
		for (auto const& mat : trans.crafting.materials) {
			textString.append(std::to_string(mat.second) + " ");
			textString.append(g_textProvider->getText(mat.first, "item", false, true) + ", ");
		}
		textString = textString.substr(0, textString.size() - 2);
		textString.append(")");

		for (auto const& mat : trans.crafting.materials) {
			if (!m_screen->getCharacterCore()->hasItem(mat.first, mat.second)) {
				m_isSelectable = false;
				break;
			}
		}
	}

	switch (type) {
	case DialogueOptionType::End: {
		auto str = g_textProvider->getText("DialogueEnd", "core", false, true);
		textString.append(textString.empty() ? str : " " + str);
		break;
	}
	case DialogueOptionType::Trade: {
		auto str = g_textProvider->getText("DialogueTrade", "core", false, true);
		textString.append(textString.empty() ? str : " " + str);
		break;
	}
	default:
		break;
	}

	std::basic_string<sf::Uint32> utf32line;
	sf::Utf8::toUtf32(textString.begin(), textString.end(), std::back_inserter(utf32line));

	m_text.setFont(*g_resourceManager->getFont(GlobalResource::FONT_TTF_DIALOGUE));
	m_text.setString(utf32line);
	m_text.setCharacterSize(GUIConstants::CHARACTER_SIZE_DIALOGUE);
	setBoundingBox(sf::FloatRect(0.f, 0.f, m_text.getLocalBounds().width, 20.f));
	setInputInDefaultView(true);
}

void DialogueOption::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_text.setPosition(pos);
}

void DialogueOption::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_text);
}

void DialogueOption::setColor(const sf::Color& color) {
	m_text.setFillColor(color);
}

bool DialogueOption::executeCrafting() {
	if (m_translation.crafting.item.empty()) return true;
	if (!m_isSelectable) return false;

	auto wScreen = dynamic_cast<WorldScreen*>(m_screen);
	for (auto const& mat : m_translation.crafting.materials) {
		wScreen->notifyItemChange(mat.first, -mat.second);
	}
	wScreen->notifyItemChange(m_translation.crafting.item, 1);

	return true;
}

void DialogueOption::onLeftClick() {
	m_isClicked = true;
}

bool DialogueOption::isClicked() {
	bool wasClicked = m_isClicked;
	m_isClicked = false;
	return wasClicked;
}

void DialogueOption::select() {
	m_text.setFillColor(m_isSelectable ? COLOR_WHITE : COLOR_ELEMENTAL);
	m_isSelected = true;
}

GameObjectType DialogueOption::getConfiguredType() const {
	return _Undefined;
}

void DialogueOption::deselect() {
	m_text.setFillColor(m_isSelectable ? COLOR_GREY : COLOR_ELEMENTAL_INACTIVE);
	m_isSelected = false;
}

bool DialogueOption::isSelected() const {
	return m_isSelected;
}

bool DialogueOption::isSelectable() const {
	return m_isSelectable;
}