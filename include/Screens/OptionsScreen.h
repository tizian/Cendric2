#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "FileIO/ConfigurationWriter.h"

#include "GUI/Button.h"
#include "GUI/Checkbox.h"
#include "GUI/Slider.h"
#include "GUI/BitmapText.h"

class OptionsScreen : public Screen {
public:
	OptionsScreen(CharacterCore* core);

	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	void execOnEnter(const Screen *previousScreen) override;
	void execOnExit(const Screen *nextScreen) override;

private:
	Button* m_backButton = nullptr;
	Button* m_applyButton = nullptr;
	Button* m_keyBindingsButton = nullptr;
	BitmapText* m_title = nullptr;

	Checkbox* m_soundCheckbox = nullptr;
	Checkbox* m_quickCastCheckbox = nullptr;
	Checkbox* m_vSyncCheckbox = nullptr;
	Checkbox* m_smoothingCheckbox = nullptr;

	BitmapText* m_fullscreen = nullptr;
	bool m_selectedFullscreenOn = false;
	Button* m_fullscreenButton = nullptr;
	Button* m_windowButton = nullptr;
	void refreshFullscreenText();

	Language m_selectedLanguage = Language::VOID;
	BitmapText* m_languageText = nullptr;
	Button* m_englishButton = nullptr;
	Button* m_germanButton = nullptr;
	Button* m_swissButton = nullptr;
	void refreshLanguageText();

	Slider* m_volumeSlider = nullptr;
};