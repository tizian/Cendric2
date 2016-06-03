#include "GUI/DialogueWindow.h"
#include "GUI/ScrollBar.h"
#include "GUI/ScrollHelper.h"
#include "GUI/SlicedSprite.h"
#include "CharacterCore.h"
#include "Map/NPC.h"
#include "GlobalResource.h"

using namespace std;

const sf::IntRect CENDRIC_TEX_POS = sf::IntRect(0, 0, 250, 250);
const std::string CENDRIC_NAME = "Cendric";

const int MAX_CHARS_PER_OPTION = 72;

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
	m_speakerSprite = sf::Sprite(*(g_resourceManager->getTexture(GlobalResource::TEX_DIALOGUE)));
	m_speakerSprite.setTextureRect(CENDRIC_TEX_POS);

	m_speakerText = new BitmapText("");
	m_speakerText->setCharacterSize(GUIConstants::CHARACTER_SIZE_L);
	m_speakerText->setColor(COLOR_LIGHT_PURPLE);

	m_dialogueText = new BitmapText("");
	m_dialogueText->setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_dialogueText->setColor(COLOR_WHITE);

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
	m_speakerSprite.setPosition(sf::Vector2f(pos.x, WINDOW_HEIGHT - 250.f));
	m_speakerText->setPosition(sf::Vector2f(pos.x + LEFT_OFFSET, pos.y + 3 * WINDOW_MARGIN));
	m_dialogueText->setPosition(sf::Vector2f(pos.x + LEFT_OFFSET, pos.y + SCROLL_WINDOW_TOP));
}

DialogueWindow::~DialogueWindow() {
	delete m_dialogueText;
	delete m_speakerText;
	delete m_scrollBar;
	delete m_scrollHelper;
}

void DialogueWindow::load(NPC* npc, WorldScreen* screen) {
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
	m_npcTexturePosition = m_npc->getNPCData().dialogueTexturePositon;
	m_npcID = m_npc->getNPCData().id;
	m_dialogueTextID = m_npc->getNPCData().textType;
	m_npcName = g_textProvider->getText(m_npcID, "npc");
}

void DialogueWindow::setNPCTalking(const std::string& text) {
	m_options.clear();
	m_speakerSprite.setTextureRect(m_npcTexturePosition);
	m_speakerText->setString(m_npcName);
	m_dialogueText->setString(g_textProvider->getCroppedText(text, m_dialogueTextID, GUIConstants::CHARACTER_SIZE_M, static_cast<int>(TEXT_WIDTH)));
}

void DialogueWindow::setCendricTalking(const std::string& text) {
	m_options.clear();
	m_speakerSprite.setTextureRect(CENDRIC_TEX_POS);
	m_speakerText->setString(CENDRIC_NAME);
	m_dialogueText->setString(g_textProvider->getCroppedText(text, m_dialogueTextID, GUIConstants::CHARACTER_SIZE_M, static_cast<int>(TEXT_WIDTH)));
}

void DialogueWindow::setNPCTrading(const std::string& text) {
	m_options.clear();
	m_speakerSprite.setTextureRect(m_npcTexturePosition);
	m_speakerSprite.setScale(sf::Vector2f(0.6f, 0.6f));
	m_speakerText->setString(m_npcName);
	m_dialogueText->setString(g_textProvider->getCroppedText(text, m_dialogueTextID, GUIConstants::CHARACTER_SIZE_M, static_cast<int>(TEXT_WIDTH)));
	delete m_merchantInterface;
	m_merchantInterface = new MerchantInterface(dynamic_cast<WorldScreen*>(m_screen), m_npcID);
	setPosition(sf::Vector2f(LEFT, TOP + 0.5f * HEIGHT));
	m_speakerSprite.setPosition(sf::Vector2f(LEFT, WINDOW_HEIGHT - 150.f));
	setHeight(0.5f * HEIGHT); 
}

void DialogueWindow::setDialogueChoice(const std::vector<std::pair<std::string, int>>& choices) {
	m_scrollBar->setScrollPosition(0.f);
	m_options.clear();
	m_dialogueText->setString("");
	m_speakerSprite.setTextureRect(CENDRIC_TEX_POS);
	m_speakerText->setString(CENDRIC_NAME);
	for (size_t i = 0; i < choices.size(); ++i) {
		DialogueOption option(choices[i].first, m_dialogueTextID, choices[i].second == -1);
		option.deselect();
		m_options.push_back(option);
	}
	m_chosenOption = 0;
	m_options[m_chosenOption].select();
}

bool DialogueWindow::updateDialogue(const sf::Time frameTime) {
	updateTime(m_dialogueTimeout, frameTime);

	if (g_inputController->isKeyJustPressed(Key::Escape) && m_dialogueTimeout == sf::Time::Zero) {
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
			setPosition(sf::Vector2f(LEFT, TOP));
			m_speakerSprite.setScale(sf::Vector2f(1.f, 1.f));
			setHeight(HEIGHT);
			m_dialogue->setNextNode(-1);
			return m_dialogue->updateWindow();
		}
		return true;
	}

	if (!m_options.empty()) {

		m_scrollBar->update(frameTime);

		int oldOption = m_chosenOption;

		updateScrolling(frameTime);

		for (size_t i = 0; i < m_options.size(); ++i) {
			if (i == m_chosenOption) {
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
			g_resourceManager->playSound(m_sound, GlobalResource::SOUND_GUI_MENUCURSOR, true);
		}

		calculateEntryPositions();

	}

	if (m_dialogueTimeout == sf::Time::Zero && m_options.empty() && (g_inputController->isMouseClickedLeft() || g_inputController->isMouseClickedRight())) {
		m_dialogue->setNextNode(-1);
		return m_dialogue->updateWindow();
	}

	if (m_dialogueTimeout == sf::Time::Zero && (chooseOption || g_inputController->isSelected())) {
		if (m_options.empty()) {
			m_dialogue->setNextNode(-1);
		}
		else {
			m_dialogue->setNextNode(m_chosenOption);
		}
		m_dialogueTimeout = DIALOGUE_TIMEOUT;
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
	float yOffset = TOP + SCROLL_WINDOW_TOP + 2 * WINDOW_MARGIN - effectiveScrollOffset;

	for (auto& it : m_options) {
		it.setBoundingBox(sf::FloatRect(xOffset, yOffset + 0.5f * GUIConstants::CHARACTER_SIZE_M, SCROLL_WINDOW_WIDTH - ScrollBar::WIDTH, 2.f * GUIConstants::CHARACTER_SIZE_M));
		it.setPosition(sf::Vector2f(xOffset, yOffset));
		yOffset += 2.f * GUIConstants::CHARACTER_SIZE_M;
	}
}

void DialogueWindow::updateScrolling(const sf::Time& frameTime) {
	if (g_inputController->isKeyJustPressed(Key::Up)) {
		m_chosenOption = std::max(m_chosenOption - 1, 0);
		DialogueOption& option = m_options[m_chosenOption];
		if (option.getPosition().y < TOP + SCROLL_WINDOW_TOP) {
			m_scrollBar->scroll(-1);
		}
		m_upActiveTime = frameTime;
		return;
	}

	if (g_inputController->isKeyJustPressed(Key::Down)) {
		m_chosenOption = std::min(m_chosenOption + 1, static_cast<int>(m_options.size()) - 1);
		DialogueOption& option = m_options[m_chosenOption];
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
		DialogueOption& option = m_options[m_chosenOption];
		m_timeSinceTick = sf::Time::Zero;
		if (option.getPosition().y < TOP) {
			m_scrollBar->scroll(-1);
		}
		return;
	}

	if (m_downActiveTime > SCROLL_TIMEOUT) {
		m_chosenOption = std::min(m_chosenOption + 1, static_cast<int>(m_options.size()) - 1);
		DialogueOption& option = m_options[m_chosenOption];
		m_timeSinceTick = sf::Time::Zero;
		if (option.getPosition().y + option.getSize().y > TOP + HEIGHT) {
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

// Dialogue Option

DialogueOption::DialogueOption(const std::string& text, const std::string& dialogueID, bool isEnd) {
	std::string textString = g_textProvider->getText(text, dialogueID);
	if (textString.size() > MAX_CHARS_PER_OPTION) {
		g_logger->logWarning("DialogueOption::DialogueOption", "You should keep option strings below 72 characters.");
	}
	if (isEnd) {
		textString.append(textString.empty() ? g_textProvider->getText("DialogueEnd") : " " + g_textProvider->getText("DialogueEnd"));
	}
	m_text.setString(textString);
	m_text.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_text.setColor(COLOR_WHITE);
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
	m_text.setColor(color);
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
	m_text.setColor(COLOR_WHITE);
	m_isSelected = true;
}

GameObjectType DialogueOption::getConfiguredType() const {
	return GameObjectType::_Undefined;
}

void DialogueOption::deselect() {
	m_text.setColor(COLOR_GREY);
	m_isSelected = false;
}

bool DialogueOption::isSelected() const {
	return m_isSelected;
}