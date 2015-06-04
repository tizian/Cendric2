#include "GUI/YesOrNoForm.h"

using namespace std;

YesOrNoForm::YesOrNoForm(const sf::FloatRect& box, const sf::Color& pressedColor, const sf::Color& mousoverColor, const sf::Color& releasedColor) 
{
	float buttonHeight = 40.f;
	float buttonGap = 20.f;
	float buttonWidth = (box.width - (buttonGap + 2.f * DIST_FROM_BORDER)) / 2.f;
	
	m_yesButton = new Button(sf::FloatRect(box.left + DIST_FROM_BORDER, box.top + (box.height - (buttonHeight + DIST_FROM_BORDER)), buttonWidth, buttonHeight), pressedColor, mousoverColor, releasedColor);
	m_noButton = new Button(sf::FloatRect(box.left + buttonGap + DIST_FROM_BORDER + buttonWidth, box.top + (box.height - (buttonHeight + DIST_FROM_BORDER)), buttonWidth, buttonHeight), pressedColor, mousoverColor, releasedColor);

	m_yesButton->setText("Yes");
	m_noButton->setText("No");

	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(box);
	setPosition(sf::Vector2f(box.left, box.top));

	m_shape = sf::RectangleShape(sf::Vector2f(box.width, box.height));
	m_shape.setPosition(box.left, box.top);

	m_shape.setOutlineThickness(2.f);
	m_shape.setFillColor(releasedColor);
	m_shape.setOutlineColor(mousoverColor);
}

YesOrNoForm::YesOrNoForm(const sf::FloatRect& box) 
{
	float buttonHeight = 40.f;
	float buttonGap = 20.f;
	float buttonWidth = (box.width - (buttonGap + 2.f * DIST_FROM_BORDER)) / 2.f;

	m_yesButton = new Button(sf::FloatRect(box.left + DIST_FROM_BORDER, box.top + (box.height - (buttonHeight + DIST_FROM_BORDER)), buttonWidth, buttonHeight));
	m_noButton = new Button(sf::FloatRect(box.left + buttonGap + DIST_FROM_BORDER + buttonWidth, box.top + (box.height - (buttonHeight + DIST_FROM_BORDER)), buttonWidth, buttonHeight));

	m_yesButton->setText("Yes");
	m_noButton->setText("No");

	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(box);
	setPosition(sf::Vector2f(box.left, box.top));

	m_shape = sf::RectangleShape(sf::Vector2f(box.width, box.height));
	m_shape.setPosition(box.left, box.top);

	m_shape.setOutlineThickness(2.f);
	m_shape.setFillColor(CENDRIC_COLOR_BLACK);
	m_shape.setOutlineColor(CENDRIC_COLOR_PURPLE);
}

YesOrNoForm::~YesOrNoForm()
{
	delete m_yesButton;
	delete m_noButton;
}

void YesOrNoForm::load()
{
	// nop
}

void YesOrNoForm::render(sf::RenderTarget& renderTarget)
{
	renderTarget.draw(m_shape);
	renderTarget.draw(m_message);

	m_yesButton->render(renderTarget);
	m_noButton->render(renderTarget);
}

void YesOrNoForm::update(const sf::Time& frameTime)
{
	m_yesButton->update(frameTime);
	m_noButton->update(frameTime);
}

void YesOrNoForm::setMessage(const std::string& msg, const sf::Color& color)
{
	m_message = BitmapText(
		g_textProvider->getText(msg),
		*g_resourceManager->getBitmapFont(ResourceID::BitmapFont_default));

	m_message.setColor(color);
	m_message.setCharacterSize(16);
	// calculate position
	m_message.setPosition(sf::Vector2f(DIST_FROM_BORDER, DIST_FROM_BORDER) + getPosition());
}

void YesOrNoForm::setMessage(const std::string& msg)
{
	setMessage(msg, sf::Color::White);
}

bool YesOrNoForm::isYesClicked()
{
	return m_yesButton->isClicked();
}

bool YesOrNoForm::isNoClicked()
{
	return m_noButton->isClicked();
}

GameObjectType YesOrNoForm::getConfiguredType() const
{
	return GameObjectType::_Form;
}