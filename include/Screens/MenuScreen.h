#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "Screens/LoadingScreen.h"
#include "Screens/SplashScreen.h"
#include "Screens/CreditsScreen.h"
#include "Screens/OptionsScreen.h"
#include "Screens/LoadGameScreen.h"
#include "Screens/SaveGameScreen.h"
#include "CharacterCore.h"
#include "GUI/Button.h"
#include "GUI/BitmapText.h"
#include "GUI/YesOrNoForm.h"

class MenuScreen : public Screen {
public:
	MenuScreen(CharacterCore* core);
	~MenuScreen();

	void execUpdate(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	void execOnEnter(const Screen *previousScreen) override;
	void execOnExit(const Screen *nextScreen) override;

private:
	sf::Sprite m_screenSpriteBackground;
	sf::Sprite m_screenSpriteForeground;
	BitmapText m_versionText;

	particles::TextureParticleSystem* m_ps_left = nullptr;
	particles::TextureParticleSystem* m_ps_right = nullptr;

	void setAllButtonsEnabled(bool value) override;

	YesOrNoForm* m_yesOrNoForm = nullptr;

	Button* m_saveGameButton = nullptr;

	// agents for the yes or no form
	void onStartNewGame();
	void onNo();
	// agents for other buttons
	void onExit();
	void onResume();
	void onNewGame();
	void onLoadGame();
	void onSaveGame();
	void onOptions();
	void onCredits();
};