#include "GUI/DialogueWindow.h"
#include "CharacterCore.h"

using namespace std;

const sf::Vector2f TEXT_OFFSET = sf::Vector2f(255.f, 10.f);
const sf::FloatRect BOX = sf::FloatRect(0.f, WINDOW_HEIGHT - 200.f, WINDOW_WIDTH, 200.f);
const int CHAR_SIZE_SPEAKER = 16;
const int CHAR_SIZE_DIALOGUE = 12;

DialogueWindow::DialogueWindow() : Window(BOX, WindowOrnamentStyle::LARGE, sf::Color(0, 0, 0, 100), sf::Color(0, 0, 0, 100), sf::Color::White) {
	m_speakerSprite = sf::Sprite(*(g_resourceManager->getTexture(ResourceID::Texture_dialogue)));
	m_speakerSprite.setTextureRect(m_cendricTexturePosition);
	m_speakerText = new BitmapText(L"");
	m_speakerText->setCharacterSize(CHAR_SIZE_SPEAKER);
	m_speakerText->setColor(CENDRIC_COLOR_LIGHT_PURPLE);
	m_dialogueText = new BitmapText(L"");
	m_dialogueText->setCharacterSize(CHAR_SIZE_DIALOGUE);
	m_dialogueText->setColor(sf::Color::White);
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
	//g_textProvider->releaseDialogueText();
}

void DialogueWindow::load(const NPCBean& npcBean, GameScreen* screen) {
	setNPC(npcBean);
	setDialogue(npcBean.dialogueID, screen);
}

void DialogueWindow::setDialogue(const std::string& dialogueID, GameScreen* screen) {
	//g_textProvider->releaseDialogueText();
	delete m_dialogue;
	m_dialogue = new Dialogue();
	m_dialogue->load(dialogueID, screen, this);
	m_screen = screen;

	if (m_dialogue->getID().size() > 4) {
		// removing the .lua
		std::string dialogueTranslations = m_dialogue->getID().substr(0, m_dialogue->getID().size() - 4);
		// adding .csv
		dialogueTranslations = dialogueTranslations + ".csv";
		// if file doesn't exist, text provider will do nothing.
		//g_textProvider->loadDialogueText(dialogueTranslations);
	}

	if (!m_dialogue->updateWindow()) {
		g_logger->logError("DialogueWindow", "No node in current dialogue");
	}
}

void DialogueWindow::setNPC(const NPCBean& npc) {
	m_npcTexturePosition = npc.dialogueTexturePositon;
	m_npcName = g_textProvider->getText(npc.id);
	m_npcID = npc.id;
}

void DialogueWindow::setNPCTalking(const std::string& text) {
	m_options.clear();
	m_speakerSprite.setTextureRect(m_npcTexturePosition);
	m_speakerText->setString(m_npcName);
	m_dialogueText->setString(g_textProvider->getCroppedText(text, CHAR_SIZE_DIALOGUE, WINDOW_WIDTH - 250 - 2 * static_cast<int>(TEXT_OFFSET.x)));
}

void DialogueWindow::setCendricTalking(const std::string& text) {
	m_options.clear();
	m_speakerSprite.setTextureRect(m_cendricTexturePosition);
	m_speakerText->setString(m_cendricName);
	m_dialogueText->setString(g_textProvider->getCroppedText(text, CHAR_SIZE_DIALOGUE, WINDOW_WIDTH - 250 - 2 * static_cast<int>(TEXT_OFFSET.x)));
}

void DialogueWindow::setNPCTrading(const std::string& text) {
	m_options.clear();
	m_speakerSprite.setTextureRect(m_npcTexturePosition);
	m_speakerSprite.setScale(sf::Vector2f(0.6f, 0.6f));
	m_speakerText->setString(m_npcName);
	m_dialogueText->setString(g_textProvider->getCroppedText(text, CHAR_SIZE_DIALOGUE, WINDOW_WIDTH - 250 - 2 * static_cast<int>(TEXT_OFFSET.x)));
	delete m_merchantInterface;
	m_merchantInterface = new MerchantInterface(dynamic_cast<GameScreen*>(m_screen), m_npcID);
	setPosition(sf::Vector2f(getPosition().x, getPosition().y + BOX.height / 2.f));
	m_speakerSprite.setPosition(sf::Vector2f(getPosition().x, WINDOW_HEIGHT - 150.f));
	setHeight(BOX.height / 2.f);
}

void DialogueWindow::setDialogueChoice(const std::vector<std::pair<std::string, int>>& choices) {
	m_options.clear();
	m_dialogueText->setString("");
	m_speakerSprite.setTextureRect(m_cendricTexturePosition);
	m_speakerText->setString(m_cendricName);
	for (int i = 0; i < choices.size(); i++) {
		DialogueOption option(choices[i].first, i);
		option.deselect();
		m_options.push_back(option);
	}
	m_chosenOption = 0;
	m_options[m_chosenOption].select();
}

bool DialogueWindow::updateDialogue(const sf::Time frameTime) {
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
	}

	if (m_options.empty() && (g_inputController->isMouseClickedLeft() || g_inputController->isMouseClickedRight())) {
		m_dialogue->setNextNode(-1);
		return m_dialogue->updateWindow();
	}

	if (chooseOption || g_inputController->isKeyJustPressed(Key::Confirm) || g_inputController->isKeyJustPressed(Key::Interact)) {
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

DialogueOption::DialogueOption(std::string text, int nr) : m_text(g_textProvider->getText(text)) {
	m_text.setCharacterSize(CHAR_SIZE_DIALOGUE);
	m_text.setColor(sf::Color::White);
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
	m_text.setColor(sf::Color::White);
	m_isSelected = true;
}

GameObjectType DialogueOption::getConfiguredType() const {
	return GameObjectType::_Undefined;
}

void DialogueOption::deselect() {
	m_text.setColor(CENDRIC_COLOR_GREY);
	m_isSelected = false;
}

bool DialogueOption::isSelected() const {
	return m_isSelected;
}
