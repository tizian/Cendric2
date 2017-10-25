#include "Screens/CreditsScreen.h"
#include "Screens/MenuScreen.h"
#include "GlobalResource.h"
#include "GUI/Button.h"
#include "GUI/GUIConstants.h"

const float CREDITS_VELOCITY_Y = -36.f;

const std::string SPRITE_PATH = "res/texture/screens/screen_credits.png";

CreditsScreen::CreditsScreen(CharacterCore* core) : Screen(core) {
	g_resourceManager->loadTexture(SPRITE_PATH, ResourceType::Unique, this);
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(SPRITE_PATH)));
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

void CreditsScreen::execOnEnter() {
	// text
	m_title = new BitmapText(g_textProvider->getText("Credits"), TextStyle::Shadowed);
	m_title->setCharacterSize(GUIConstants::CHARACTER_SIZE_XXXL);
	m_title->setPosition(sf::Vector2f(0.5f * (WINDOW_WIDTH - m_title->getLocalBounds().width), 50.f));

	m_credits = new BitmapText(g_textProvider->getText("CreditsText"), TextAlignment::Center);
	m_credits->setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_credits->setPosition(sf::Vector2f(0.5f * (WINDOW_WIDTH - m_credits->getLocalBounds().width), 0.5f * WINDOW_HEIGHT));

	// add buttons
	Button* button = new Button(sf::FloatRect(60, WINDOW_HEIGHT - 80, 200, 50), GUIOrnamentStyle::SMALL);
	button->setText("Back");
	button->setOnClick(std::bind(&CreditsScreen::onBack, this));
	addObject(button);

	g_resourceManager->playMusic(GlobalResource::MUSIC_MAIN, false);
}

void CreditsScreen::execOnExit() {
	g_resourceManager->deleteUniqueResources(this);
	delete m_title;
	delete m_credits;
}

void CreditsScreen::onBack() {
	setNextScreen(new MenuScreen(m_characterCore));
}