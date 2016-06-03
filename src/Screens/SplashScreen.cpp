#include "Screens/SplashScreen.h"
#include "Screens/ScreenManager.h"

using namespace std;

const std::string SPRITE_PATH = "res/assets/screens/screen_splash.png";

SplashScreen::SplashScreen() : Screen(nullptr) {
	g_resourceManager->loadTexture(SPRITE_PATH, ResourceType::Unique, this);
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(SPRITE_PATH)));
}

void SplashScreen::execOnEnter(const Screen* previousScreen) {
	// add burning fire baskets
	FireBasket* fireBasket1 = new FireBasket();
	FireBasket* fireBasket2 = new FireBasket();
	fireBasket1->setPosition(sf::Vector2f(60.f, -80.f));
	fireBasket2->setPosition(sf::Vector2f(1028.f, -80.f));
	addObject(fireBasket1);
	addObject(fireBasket2);

	// add version nr
	m_versionText.setString("Cendric v" + std::string(CENDRIC_VERSION_NR));
	m_versionText.setCharacterSize(8);
	m_versionText.setColor(COLOR_WHITE);
	m_versionText.setPosition(
		(WINDOW_WIDTH - m_versionText.getLocalBounds().width) / 2,
		WINDOW_HEIGHT - 18.f);
}

void SplashScreen::execUpdate(const sf::Time& frameTime) {
	if (g_inputController->isKeyActive(Key::Confirm) || g_inputController->isMouseJustPressedLeft()) {
		setNextScreen(new MenuScreen(nullptr));
		return;
	}
	if (g_inputController->isKeyActive(Key::Escape)) {
		m_screenManager->requestQuit();
		return;
	}
	updateObjects(GameObjectType::_Undefined, frameTime);
}

void SplashScreen::render(sf::RenderTarget& renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_screenSprite);
	renderObjects(GameObjectType::_Undefined, renderTarget);
	renderObjects(GameObjectType::_Interface, renderTarget);
	renderTarget.draw(m_versionText);
}

void SplashScreen::execOnExit(const Screen *nextScreen) {
	g_resourceManager->deleteUniqueResources(this);
}