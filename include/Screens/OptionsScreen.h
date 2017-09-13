#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "FileIO/ConfigurationWriter.h"

#include "GUI/Button.h"
#include "GUI/Checkbox.h"
#include "GUI/Slider.h"
#include "GUI/ArrowSelector.h"
#include "GUI/BitmapText.h"

class OptionsScreen final : public Screen {
public:
	OptionsScreen(CharacterCore* core);

	void execUpdate(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	void execOnEnter(const Screen *previousScreen) override;
	void execOnExit(const Screen *nextScreen) override;

private:
	BitmapText* m_title = nullptr;

	Checkbox* m_soundCheckbox = nullptr;
	Checkbox* m_quickCastCheckbox = nullptr;
	Checkbox* m_smoothingCheckbox = nullptr;
	Checkbox* m_displayHintsCheckbox = nullptr;
	Checkbox* m_displayDamageNumbersCheckbox = nullptr;
	Checkbox* m_autotargetCheckbox = nullptr;
	Checkbox* m_vSyncCheckbox = nullptr;
	Checkbox* m_limitFPSCheckbox = nullptr;

	Slider* m_maxFPSSlider = nullptr;
	Slider* m_volumeSoundSlider = nullptr;
	Slider* m_volumeMusicSlider = nullptr;

	ArrowSelector* m_languageSelector = nullptr;
	ArrowSelector* m_displayModeSelector = nullptr;

	// button agents
	void onBack();
	void onApply();
	void checkSoundSlider();
	void checkFPSSlider();
};