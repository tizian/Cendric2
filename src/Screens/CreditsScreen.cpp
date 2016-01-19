#include "Screens/CreditsScreen.h"
#include "Screens/MenuScreen.h"

using namespace std;

CreditsScreen::CreditsScreen(CharacterCore* core) : Screen(core) {
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_screen_credits)));
}

void CreditsScreen::execUpdate(const sf::Time& frameTime) {
	if (g_inputController->isKeyActive(Key::Escape) || m_backButton->isClicked()) {
		setNextScreen(new MenuScreen(m_characterCore));
		return;
	}
	updateObjects(GameObjectType::_Button, frameTime);
}

void CreditsScreen::render(sf::RenderTarget &renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_screenSprite);
	renderTarget.draw(*m_title);
	renderTarget.draw(*m_credits);
	renderObjects(GameObjectType::_Button, renderTarget);
}

void CreditsScreen::execOnEnter(const Screen *previousScreen) {
	// text
	m_title = new BitmapText(g_textProvider->getText("Credits"));
	m_title->setCharacterSize(50);
	m_title->setPosition(sf::Vector2f((WINDOW_WIDTH - m_title->getLocalBounds().width) / 2.f, 50.f));

	int creditsWidth = 400;
	int creditsCharSize = 12;
	m_credits = new BitmapText(g_textProvider->getCroppedText("CreditsText", creditsCharSize, creditsWidth));
	m_credits->setCharacterSize(creditsCharSize);
	m_credits->setPosition(sf::Vector2f((WINDOW_WIDTH - creditsWidth) / 2.f, 150.f));

	// add buttons
	m_backButton = new Button(sf::FloatRect(60, WINDOW_HEIGHT - 100, 200, 50));
	m_backButton->setText("Back");
	addObject(m_backButton);
}

void CreditsScreen::execOnExit(const Screen *nextScreen) {
	g_resourceManager->deleteResource(ResourceID::Texture_screen_credits);
	delete m_title;
	delete m_credits;
}