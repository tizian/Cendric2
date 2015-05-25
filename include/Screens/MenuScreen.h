#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "Screens/LoadingScreen.h"
#include "Screens/SplashScreen.h"
#include "Misc/FireBasket.h"
#include "CharacterCore.h"
#include "Button.h"

class MenuScreen : public Screen
{
public:
	MenuScreen(CharacterCore* core);

	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	void execOnEnter(const Screen *previousScreen) override;
	void execOnExit(const Screen *nextScreen) override;

private:
	sf::Sprite m_screenSprite;

	// buttons on this screen
	Button* m_newGameButton;
	Button* m_loadGameButton;
	Button* m_startGameButton;
	Button* m_exitButton;
};