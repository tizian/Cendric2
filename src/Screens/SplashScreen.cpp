#include "Screens/SplashScreen.h"

using namespace std;

SplashScreen::SplashScreen() : Screen(nullptr) {
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_screen_splash)));
}

void SplashScreen::execOnEnter(const Screen* previousScreen) {
	// add burning fire baskets
	FireBasket* fireBasket1 = new FireBasket();
	FireBasket* fireBasket2 = new FireBasket();
	fireBasket1->setPosition(sf::Vector2f(60.f, 120.f));
	fireBasket2->setPosition(sf::Vector2f(1028.f, 120.f));
	addObject(fireBasket1);
	addObject(fireBasket2);

	// add version nr
	m_versionText.setString("Cendric v" + std::string(CENDRIC_VERSION_NR));
	m_versionText.setCharacterSize(8);
	m_versionText.setColor(sf::Color::White);
	m_versionText.setPosition(
		(WINDOW_WIDTH - m_versionText.getLocalBounds().width) / 2,
		WINDOW_HEIGHT - 18.f);
}

Screen* SplashScreen::update(const sf::Time& frameTime) {
	if (g_inputController->isKeyActive(Key::Escape) || g_inputController->isMouseJustPressedLeft()) {
		return new MenuScreen(nullptr);
	}
	updateObjects(GameObjectType::_Undefined, frameTime);
	return this;
}

void SplashScreen::render(sf::RenderTarget &renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_screenSprite);
	renderObjects(GameObjectType::_Undefined, renderTarget);
	renderObjects(GameObjectType::_Interface, renderTarget);
	renderTarget.draw(m_versionText);
}

void SplashScreen::execOnExit(const Screen *nextScreen) {
	g_resourceManager->deleteResource(ResourceID::Texture_screen_splash);
}