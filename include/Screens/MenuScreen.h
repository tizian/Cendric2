#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "Screens/LoadingScreen.h"
#include "Screens/SplashScreen.h"
#include "Screens/CreditsScreen.h"
#include "Misc/FireBasket.h"
#include "CharacterCore.h"
#include "GUI/Button.h"
#include "GUI/BitmapText.h"
#include "GUI/YesOrNoForm.h"

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

	// enables / disables all buttons on this screen
	void setAllButtonsEnabled(bool value);

	// only used when character core is not null
	Button* m_resumeGameButton = nullptr;
	Button* m_newGameButton = nullptr;
	Button* m_loadGameButton = nullptr;
	Button* m_saveGameButton = nullptr;
	Button* m_optionsButton = nullptr;
	Button* m_creditsButton = nullptr;
	Button* m_exitButton = nullptr;

	YesOrNoForm* m_yesOrNoForm = nullptr;
	CharacterCore* m_newCharacterCore = nullptr;
};