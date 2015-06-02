#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "Screens/LoadingScreen.h"
#include "Screens/SplashScreen.h"
#include "Misc/FireBasket.h"
#include "CharacterCore.h"
#include "Button.h"

#include "GUI/BitmapText.h"

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

	BitmapText m_testText;

	// only used when character core is not null
	Button* m_resumeGameButton;
	Button* m_newGameButton; 
	Button* m_loadGameButton;
	Button* m_saveGameButton;
	Button* m_optionsButton;
	Button* m_creditsButton;
	Button* m_exitButton;
};