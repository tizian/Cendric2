#include "GUI/YesOrNoForm.h"

using namespace std;

// determines the distance of the message text and the buttons from border
const float DIST_FROM_BORDER = 10.f;

YesOrNoForm::YesOrNoForm(const sf::FloatRect& box) : GameObject() {
	m_window = new Window(box, GUIOrnamentStyle::NONE);

	float buttonHeight = 50.f;
	float buttonGap = 20.f;
	float buttonWidth = (box.width - (buttonGap + 2.f * DIST_FROM_BORDER)) / 2.f;

	m_yesButton = new Button(sf::FloatRect(box.left + DIST_FROM_BORDER, box.top + (box.height - (buttonHeight + DIST_FROM_BORDER)), buttonWidth, buttonHeight));
	m_noButton = new Button(sf::FloatRect(box.left + buttonGap + DIST_FROM_BORDER + buttonWidth, box.top + (box.height - (buttonHeight + DIST_FROM_BORDER)), buttonWidth, buttonHeight));

	m_yesButton->setText("Yes");
	m_noButton->setText("No");

	m_buttonGroup = new ButtonGroup();
	m_buttonGroup->setHorizontal(true);

	m_buttonGroup->addButton(m_yesButton);
	m_buttonGroup->addButton(m_noButton);

	setBoundingBox(box);
	setPosition(sf::Vector2f(box.left, box.top));
}

YesOrNoForm::~YesOrNoForm() {
	delete m_window;
	delete m_buttonGroup;
}

void YesOrNoForm::render(sf::RenderTarget& renderTarget) {
	m_window->render(renderTarget);
	renderTarget.draw(m_message);

	m_buttonGroup->render(renderTarget);
}

void YesOrNoForm::update(const sf::Time& frameTime) {
	m_buttonGroup->update(frameTime);

	if (m_yesButton->isClicked() || m_noButton->isClicked()) {
		setDisposed();
	}
}

void YesOrNoForm::setMessage(const std::string& msg, const sf::Color& color) {
	int characterSize = 16;
	m_message = BitmapText(
		g_textProvider->getCroppedText(msg, characterSize, static_cast<int>(m_window->getSize().x - (2 * DIST_FROM_BORDER))));

	m_message.setColor(color);
	m_message.setCharacterSize(characterSize);
	// calculate position
	m_message.setPosition(sf::Vector2f(DIST_FROM_BORDER, DIST_FROM_BORDER) + getPosition());
}

void YesOrNoForm::setMessage(const std::string& msg) {
	setMessage(msg, COLOR_WHITE);
}

void YesOrNoForm::setOnYesClicked(const std::function<void()>& agent) {
	m_yesButton->setOnClick(agent);
}

void YesOrNoForm::setOnNoClicked(const std::function<void()>& agent) {
	m_noButton->setOnClick(agent);
}

GameObjectType YesOrNoForm::getConfiguredType() const {
	return _Form;
}