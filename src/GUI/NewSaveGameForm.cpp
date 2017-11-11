#include "GUI/NewSaveGameForm.h"
#include "GUI/GUIConstants.h"

// determines the distance of the message text and the buttons from border
const float DIST_FROM_BORDER = 10.f;
const int MAX_NAME_LENGTH = 25;

NewSaveGameForm::NewSaveGameForm(const sf::FloatRect& box) : GameObject() {
	m_window = new Window(box, GUIOrnamentStyle::NONE);
	float buttonHeight = 50.f;
	float buttonGap = 20.f;
	float buttonWidth = (box.width - (buttonGap + 2.f * DIST_FROM_BORDER)) / 2.f;

	m_okButton = new Button(sf::FloatRect(box.left + DIST_FROM_BORDER, box.top + (box.height - (buttonHeight + DIST_FROM_BORDER)), buttonWidth, buttonHeight));
	m_cancelButton = new Button(sf::FloatRect(box.left + buttonGap + DIST_FROM_BORDER + buttonWidth, box.top + (box.height - (buttonHeight + DIST_FROM_BORDER)), buttonWidth, buttonHeight));

	m_okButton->setText("Okay");
	m_cancelButton->setText("Cancel");

	setBoundingBox(box);
	setPosition(sf::Vector2f(box.left, box.top));

	// message
	m_message = BitmapText(
		g_textProvider->getCroppedText("MessageNewSaveGame", GUIConstants::CHARACTER_SIZE_L, static_cast<int>(m_window->getSize().x - (2 * DIST_FROM_BORDER))));
	m_message.setColor(COLOR_WHITE);
	m_message.setCharacterSize(GUIConstants::CHARACTER_SIZE_L);
	// calculate position
	m_message.setPosition(sf::Vector2f(DIST_FROM_BORDER, DIST_FROM_BORDER) + getPosition());

	m_savegameName = "";
	m_savegameNameText.setString(m_savegameName);
	m_savegameNameText.setColor(COLOR_LIGHT_PURPLE);
	m_savegameNameText.setCharacterSize(16);
	m_savegameNameText.setPosition(sf::Vector2f(2 * DIST_FROM_BORDER, box.height - (buttonHeight + DIST_FROM_BORDER + 50)) + getPosition());

	g_inputController->startReadingText();
}

NewSaveGameForm::~NewSaveGameForm() {
	delete m_window;
	delete m_okButton;
	delete m_cancelButton;
	g_inputController->stopReadingText();
}

void NewSaveGameForm::render(sf::RenderTarget& renderTarget) {
	m_window->render(renderTarget);
	renderTarget.draw(m_message);
	renderTarget.draw(m_savegameNameText);

	m_okButton->render(renderTarget);
	m_cancelButton->render(renderTarget);
}

const std::string& NewSaveGameForm::getSavegameName() const {
	return m_savegameName;
}

void NewSaveGameForm::update(const sf::Time& frameTime) {
	g_inputController->cropReadText(MAX_NAME_LENGTH);
	m_savegameName = g_inputController->getReadText();
	m_savegameNameText.setString(m_savegameName);
	m_okButton->setEnabled(m_savegameName.size() < MAX_NAME_LENGTH && m_savegameName.size() > 0);
	m_okButton->update(frameTime);
	m_cancelButton->update(frameTime);

	if (m_okButton->isEnabled() && g_inputController->isKeyJustPressed(Key::Confirm)) {
		m_executeOnOk();
		g_inputController->lockAction();
		setDisposed();
	}
	if (m_cancelButton->isClicked() || m_okButton->isClicked())  {
		setDisposed();
	}
}

void NewSaveGameForm::setOnOkClicked(const std::function<void()>& agent) {
	m_executeOnOk = agent;
	m_okButton->setOnClick(agent);
}

void NewSaveGameForm::setOnCancelClicked(const std::function<void()>& agent) {
	m_cancelButton->setOnClick(agent);
}

GameObjectType NewSaveGameForm::getConfiguredType() const {
	return GameObjectType::_Form;
}
