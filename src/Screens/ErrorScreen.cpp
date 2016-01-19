#include "Screens/ErrorScreen.h"

using namespace std;

ErrorScreen::ErrorScreen(CharacterCore* core) : Screen(core) {
	ErrorID error = g_resourceManager->pollError()->first;
	switch (error) {
	case ErrorID::Error_fileNotFound:
		m_screenResource = ResourceID::Texture_screen_error_fileNotFound;
		break;
	case ErrorID::Error_dataCorrupted:
		m_screenResource = ResourceID::Texture_screen_error_dataCorrupted;
		break;
	default:
		// unexpected
		g_logger->logError("ErrorScreen", "Error screen has been set without an error occurring");
		m_screenResource = ResourceID::Texture_screen_error_dataCorrupted;
		break;
	}
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(m_screenResource)));
	m_errorText = BitmapText(
		g_resourceManager->pollError()->second,
		*g_resourceManager->getBitmapFont(ResourceID::BitmapFont_default));
	m_errorText.setColor(sf::Color::Red);
	m_errorText.setPosition(sf::Vector2f(64, 518));
	m_errorText.setCharacterSize(12);
}

void ErrorScreen::render(sf::RenderTarget &renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_screenSprite);
	renderTarget.draw(m_errorText);
}

void ErrorScreen::execUpdate(const sf::Time& frameTime) {
	if (g_inputController->isKeyActive(Key::Escape)) {
		m_requestQuit = true;
	}
}

void ErrorScreen::execOnEnter(const Screen *previousScreen) {
}

void ErrorScreen::execOnExit(const Screen *nextScreen) {
	g_resourceManager->deleteResource(m_screenResource);
}