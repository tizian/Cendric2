#include "Misc/PartyForm.h"

using namespace std;

// determines the distance of the message text and the buttons from border
const float DIST_FROM_BORDER = 10.f;

PartyForm::PartyForm(const sf::FloatRect& box) : GameObject() {
	m_window = new Window(box, GUIOrnamentStyle::NONE);
	float buttonHeight = 50.f;
	float buttonGap = 20.f;
	float buttonWidth = (box.width - (buttonGap + 2.f * DIST_FROM_BORDER)) / 2.f;

	m_okButton = new Button(sf::FloatRect(box.left + DIST_FROM_BORDER, box.top + (box.height - (buttonHeight + DIST_FROM_BORDER)), buttonWidth, buttonHeight));

	m_okButton->setText("Okay");

	setBoundingBox(box);
	setPosition(sf::Vector2f(box.left, box.top));

	// message
	int characterSize = 16;
	m_message = BitmapText(
		g_textProvider->getCroppedText("MessageEnterPartyID", characterSize, static_cast<int>(m_window->getSize().x - (2 * DIST_FROM_BORDER))));
	m_message.setColor(COLOR_WHITE);
	m_message.setCharacterSize(characterSize);
	// calculate position
	m_message.setPosition(sf::Vector2f(DIST_FROM_BORDER, DIST_FROM_BORDER) + getPosition());

	m_enteredString = "";
	m_enteredStringText.setString(m_enteredString);
	m_enteredStringText.setColor(COLOR_LIGHT_PURPLE);
	m_enteredStringText.setCharacterSize(16);
	m_enteredStringText.setPosition(sf::Vector2f(2 * DIST_FROM_BORDER, box.height - (buttonHeight + DIST_FROM_BORDER + 50)) + getPosition());

	g_inputController->startReadingText();

	m_executeOnOk = std::bind(&PartyForm::nop, this);
}

PartyForm::~PartyForm() {
	delete m_window;
	delete m_okButton;
	g_inputController->stopReadingText();
}

void PartyForm::render(sf::RenderTarget& renderTarget) {
	m_window->render(renderTarget);
	renderTarget.draw(m_message);
	renderTarget.draw(m_enteredStringText);

	m_okButton->render(renderTarget);
}

const std::string& PartyForm::getEnteredString() const {
	return m_enteredString;
}

void PartyForm::update(const sf::Time& frameTime) {
	m_enteredString = g_inputController->getReadText();
	m_enteredStringText.setString(m_enteredString);
	m_okButton->setEnabled(m_enteredString.size() < 5 && m_enteredString.size() > 0);
	m_okButton->update(frameTime);

	if (m_okButton->isEnabled() && g_inputController->isKeyJustPressed(Key::Confirm)) {
		m_executeOnOk();
		g_inputController->lockAction();
		setDisposed();
	}
	if (m_okButton->isClicked()) {
		setDisposed();
	}
}

void PartyForm::setOnOkClicked(const std::function<void()>& agent) {
	m_executeOnOk = agent;
	m_okButton->setOnClick(agent);
}

GameObjectType PartyForm::getConfiguredType() const {
	return GameObjectType::_Form;
}
