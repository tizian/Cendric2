#include "ErrorScreen.h"

using namespace std;

ErrorScreen::ErrorScreen()
{
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_screen_error)));
}

void ErrorScreen::render(sf::RenderTarget &renderTarget)
{
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_screenSprite);
}

void ErrorScreen::onEnter(Screen *previousScreen)
{
}

void ErrorScreen::onExit(Screen *nextScreen)
{
	g_resourceManager->deleteResource(ResourceID::Texture_screen_error);
}