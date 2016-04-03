#include "GUI/DialogueWindow.h"
#include "CharacterCore.h"
#include "Map/NPC.h"

using namespace std;

const sf::Vector2f TEXT_OFFSET = sf::Vector2f(270.f, 10.f);
const int TEXT_WIDTH = WINDOW_WIDTH - static_cast<int>(TEXT_OFFSET.x) - 100;
const sf::FloatRect BOX = sf::FloatRect(0.f, WINDOW_HEIGHT - 200.f, WINDOW_WIDTH, 200.f);
const int CHAR_SIZE_SPEAKER = 16;
const int CHAR_SIZE_DIALOGUE = 12;
const sf::IntRect CENDRIC_TEX_POS = sf::IntRect(0, 0, 250, 250);
const std::string CENDRIC_NAME = "Cendric";

DialogueWindow::DialogueWindow() : Window(BOX, WindowOrnamentStyle::FANCY, sf::Color(0, 0, 0, 150), sf::Color(0, 0, 0, 150), COLOR_WHITE) {
	m_speakerSprite = sf::Sprite(*(g_resourceManager->getTexture(ResourceID::Texture_dialogue)));
	m_speakerSprite.setTextureRect(CENDRIC_TEX_POS);
	m_speakerText = new BitmapText("");
	m_speakerText->setCharacterSize(CHAR_SIZE_SPEAKER);
	m_speakerText->setColor(COLOR_LIGHT_PURPLE);
	m_dialogueText = new BitmapText("");
	m_dialogueText->setCharacterSize(CHAR_SIZE_DIALOGUE);
	m_dialogueText->setColor(COLOR_WHITE);
	setPosition(getPosition());
}

void DialogueWindow::setPosition(const sf::Vector2f& pos) {
	Window::setPosition(pos);
	m_speakerSprite.setPosition(sf::Vector2f(pos.x, WINDOW_HEIGHT - 250.f));
	m_speakerText->setPosition(sf::Vector2f(pos.x + TEXT_OFFSET.x, pos.y + TEXT_OFFSET.y));
	m_dialogueText->setPosition(sf::Vector2f(pos.x + TEXT_OFFSET.x, pos.y + TEXT_OFFSET.y + 30.f));
}

DialogueWindow::~DialogueWindow() {
	delete m_dialogueText;
	delete m_speakerText;
	g_resourceManager->deleteResource(ResourceID::Texture_dialogue);
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
	
	if (m_dialogue->getID().size() > 4) {
		// removing the .lua
		m_dialogueTextID = m_dialogue->getID().substr(0, m_dialogue->getID().size() - 4);
		// removing everything from /
		if (m_dialogueTextID.find('/') != std::string::npos) {
			m_dialogueTextID = m_dialogueTextID.substr(m_dialogueTextID.find_last_of('/') + 1, m_dialogueTextID.size());
		}
	}

	m_dialogue->updateWindow();
}

void DialogueWindow::setNPC(NPC* npc) {
	m_npc = npc;
	m_npcTexturePosition = m_npc->getNPCData().dialogueTexturePositon;
	m_npcID = m_npc->getNPCData().id;
	m_npcName = g_textProvider->getText(m_npcID, "npc");
}

void DialogueWindow::setNPCTalking(const std::string& text) {
	m_options.clear();
	m_speakerSprite.setTextureRect(m_npcTexturePosition);
	m_speakerText->setString(m_npcName);
	m_dialogueText->setString(g_textProvider->getCroppedText(text, m_dialogueTextID, CHAR_SIZE_DIALOGUE, TEXT_WIDTH));
}

void DialogueWindow::setCendricTalking(const std::string& text) {
	m_options.clear();
	m_speakerSprite.setTextureRect(CENDRIC_TEX_POS);
	m_speakerText->setString(CENDRIC_NAME);
	m_dialogueText->setString(g_textProvider->getCroppedText(text, m_dialogueTextID, CHAR_SIZE_DIALOGUE, TEXT_WIDTH));
}

void DialogueWindow::setNPCTrading(const std::string& text) {
	m_options.clear();
	m_speakerSprite.setTextureRect(m_npcTexturePosition);
	m_speakerSprite.setScale(sf::Vector2f(0.6f, 0.6f));
	m_speakerText->setString(m_npcName);
	m_dialogueText->setString(g_textProvider->getCroppedText(text, m_dialogueTextID, CHAR_SIZE_DIALOGUE, TEXT_WIDTH));
	delete m_merchantInterface;
	m_merchantInterface = new MerchantInterface(dynamic_cast<WorldScreen*>(m_screen), m_npcID);
	setPosition(sf::Vector2f(getPosition().x, getPosition().y + BOX.height / 2.f));
	m_speakerSprite.setPosition(sf::Vector2f(getPosition().x, WINDOW_HEIGHT - 150.f));
	setHeight(BOX.height / 2.f); 
}

void DialogueWindow::setDialogueChoice(const std::vector<std::pair<std::string, int>>& choices) {
	m_options.clear();
	m_dialogueText->setString("");
	m_speakerSprite.setTextureRect(CENDRIC_TEX_POS);
	m_speakerText->setString(CENDRIC_NAME);
	for (size_t i = 0; i < choices.size(); ++i) {
		DialogueOption option(choices[i].first, m_dialogueTextID, static_cast<int>(i), choices[i].second == -1);
		option.deselect();
		m_options.push_back(option);
	}
	m_chosenOption = 0;
	m_options[m_chosenOption].select();
}

bool DialogueWindow::updateDialogue(const sf::Time frameTime) {
	if (g_inputController->isKeyJustPressed(Key::Escape)) {
		g_inputController->lockAction();
		// check if we can leave the window
		if (m_merchantInterface != nullptr) {
			return false;
		}
		if (m_dialogue->isEndable()) {
			return false;
		}
	}

	bool chooseOption = false;

	if (m_merchantInterface != nullptr) {
		m_merchantInterface->update(frameTime);
		if (m_merchantInterface->isCancelled()) {
			delete m_merchantInterface;
			m_merchantInterface = nullptr;
			setPosition(sf::Vector2f(BOX.left, BOX.top));
			m_speakerSprite.setScale(sf::Vector2f(1.f, 1.f));
			setPosition(getPosition());
			setHeight(BOX.height);
			m_dialogue->setNextNode(-1);
			return m_dialogue->updateWindow();
		}
		return true;
	}
	if (!m_options.empty()) {
		int oldOption = m_chosenOption;
		if (g_inputController->isScrolledUp()) {
			m_chosenOption = std::max(m_chosenOption - 1, 0);
		}
		else if (g_inputController->isScrolledDown()) {
			m_chosenOption = std::min(m_chosenOption + 1, static_cast<int>(m_options.size()) - 1);
		}
		for (size_t i = 0; i < m_options.size(); ++i) {
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
		}
	}

	if (m_options.empty() && (g_inputController->isMouseClickedLeft() || g_inputController->isMouseClickedRight())) {
		m_dialogue->setNextNode(-1);
		return m_dialogue->updateWindow();
	}

	if (chooseOption || g_inputController->isSelected()) {
		if (m_options.empty()) {
			m_dialogue->setNextNode(-1);
		}
		else {
			m_dialogue->setNextNode(m_chosenOption);
		}
		return m_dialogue->updateWindow();
	}
	return true;
}

void DialogueWindow::render(sf::RenderTarget& renderTarget) {
	Window::render(renderTarget);
	renderTarget.draw(*m_speakerText);
	if (!m_options.empty()) {
		for (auto& it : m_options) {
			it.render(renderTarget);
		}
	}
	else {
		renderTarget.draw(*m_dialogueText);
	}

	if (m_merchantInterface != nullptr) {
		m_merchantInterface->render(renderTarget);
	}

	renderTarget.draw(m_speakerSprite);
}

// Dialogue Option

DialogueOption::DialogueOption(const std::string& text, const std::string& dialogueID, int nr, bool isEnd) {
	std::string textString = g_textProvider->getText(text, dialogueID);
	if (isEnd) {
		textString.append(textString.empty() ? g_textProvider->getText("DialogueEnd") : " " + g_textProvider->getText("DialogueEnd"));
	}
	m_text.setString(textString);
	m_text.setCharacterSize(CHAR_SIZE_DIALOGUE);
	m_text.setColor(COLOR_WHITE);
	setBoundingBox(sf::FloatRect(0.f, 0.f, m_text.getLocalBounds().width, 20.f));
	setPosition(sf::Vector2f(TEXT_OFFSET.x, BOX.top + TEXT_OFFSET.y + 30.f + (nr * 20.f)));
	setInputInDefaultView(true);
}

void DialogueOption::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_text.setPosition(pos);
}

void DialogueOption::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_text);
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