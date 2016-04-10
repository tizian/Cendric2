#include "Screens/CreditsScreen.h"
#include "Screens/MenuScreen.h"

using namespace std;

const float CREDITS_VELOCITY_Y = -40.f;

CreditsScreen::CreditsScreen(CharacterCore* core) : Screen(core) {
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_screen_credits)));
}

void CreditsScreen::execUpdate(const sf::Time& frameTime) {
	if (g_inputController->isKeyActive(Key::Escape)) {
		onBack();
		return;
	}
	updateObjects(GameObjectType::_Button, frameTime);

	if (m_credits->getPosition().y + m_credits->getLocalBounds().height > WINDOW_HEIGHT / 2.f) {
		m_credits->setPosition(m_credits->getPosition() + 
			sf::Vector2f(0.f, CREDITS_VELOCITY_Y * frameTime.asSeconds()));
	}
}

void CreditsScreen::render(sf::RenderTarget &renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(*m_credits);
	renderTarget.draw(m_screenSprite);
	renderTarget.draw(*m_title);
	renderObjects(GameObjectType::_Button, renderTarget);
}

void CreditsScreen::execOnEnter(const Screen *previousScreen) {
	// text
	m_title = new BitmapText(g_textProvider->getText("Credits"));
	m_title->setCharacterSize(GUIConstants::CHARACTER_SIZE_XXL);
	m_title->setPosition(sf::Vector2f(0.5f * (WINDOW_WIDTH - m_title->getLocalBounds().width), 50.f));

	int creditsCharSize = 12;
	m_credits = new BitmapText(g_textProvider->getText("CreditsText"), TextAlignment::Center);
	m_credits->setCharacterSize(creditsCharSize);
	m_credits->setPosition(sf::Vector2f(0.5f * (WINDOW_WIDTH - m_credits->getLocalBounds().width), 0.5f * WINDOW_HEIGHT));

	// add buttons
	Button* button = new Button(sf::FloatRect(60, WINDOW_HEIGHT - 100, 200, 50));
	button->setText("Back");
	button->setOnClick(std::bind(&CreditsScreen::onBack, this));
	addObject(button);
}

void CreditsScreen::execOnExit(const Screen *nextScreen) {
	g_resourceManager->deleteResource(ResourceID::Texture_screen_credits);
	delete m_title;
	delete m_credits;
}

void CreditsScreen::onBack() {
	setNextScreen(new MenuScreen(m_characterCore));
}