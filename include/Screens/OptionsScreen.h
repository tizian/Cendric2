#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "FileIO/ConfigurationWriter.h"

#include "GUI/Button.h"
#include "GUI/BitmapText.h"
#include "Enums/EnumNames.h"

class OptionsScreen : public Screen
{
public:
	OptionsScreen(CharacterCore* core);

	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	void execOnEnter(const Screen *previousScreen) override;
	void execOnExit(const Screen *nextScreen) override;

private:
	Button* m_backButton = nullptr;
	Button* m_applyButton = nullptr;
	BitmapText* m_title = nullptr;

	BitmapText* m_sound = nullptr;
	bool m_selectedSoundOn = false;
	Button* m_soundOnButton = nullptr;
	Button* m_soundOffButton = nullptr;
	void refreshSoundText();

	Language m_selectedLanguage = Language::Void;
	BitmapText* m_languageText = nullptr;
	Button* m_englishButton = nullptr;
	Button* m_germanButton = nullptr;
	Button* m_swissButton = nullptr;
	void refreshLanguageText();

	std::vector<Button*> m_keyButtons;
	std::vector<BitmapText*> m_keyTexts;
	
	// TODO add sliders
	BitmapText* m_fps = nullptr;
	BitmapText* m_volume = nullptr;
};