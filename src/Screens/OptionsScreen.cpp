#include "Screens/OptionsScreen.h"
#include "Screens/MenuScreen.h"
#include "Screens/KeyBindingsScreen.h"

using namespace std;

OptionsScreen::OptionsScreen(CharacterCore* core) : Screen(core) {
}

void OptionsScreen::execUpdate(const sf::Time& frameTime) {
	if (g_inputController->isKeyActive(Key::Escape)) {
		onBack();
		return;
	}

	updateObjects(GameObjectType::_Button, frameTime);
	updateTooltipText(frameTime);
}

void OptionsScreen::render(sf::RenderTarget &renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(*m_title);
	renderObjects(GameObjectType::_Button, renderTarget);
	renderTooltipText(renderTarget);
}

void OptionsScreen::execOnEnter(const Screen *previousScreen) {
	// title
	m_title = new BitmapText(g_textProvider->getText("Options"));
	m_title->setCharacterSize(50);
	m_title->setPosition(sf::Vector2f((WINDOW_WIDTH - m_title->getLocalBounds().width) / 2.f, 50.f));

	float distFromTop = 150.f;
	float distFromLeft = 150.f;

	// language
	m_languageSelector = new ArrowSelector();
	m_languageSelector->setLabelText("Language");
	for (int language = 1; language < static_cast<int>(Language::MAX); ++language) {
		m_languageSelector->addOption(EnumNames::getLanguageName(static_cast<Language>(language)));
	}
	m_languageSelector->setOptionIndex(static_cast<int>(g_resourceManager->getConfiguration().language) - 1);
	m_languageSelector->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	addObject(m_languageSelector);

	distFromTop = distFromTop + 75;

	// displayMode
	m_displayModeSelector = new ArrowSelector();
	m_displayModeSelector->setLabelText("DisplayMode");
	m_displayModeSelector->addOption("Window");
	m_displayModeSelector->addOption("Fullscreen");
	m_displayModeSelector->setOptionIndex(g_resourceManager->getConfiguration().isFullscreen ? 1 : 0);
	m_displayModeSelector->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	addObject(m_displayModeSelector);

	distFromTop = distFromTop + 150;

	m_volumeSoundSlider = new Slider(0, 100);
	string volumeText = g_textProvider->getText("SoundVolume") + " (%)";
	m_volumeSoundSlider->setTextRaw(volumeText);
	m_volumeSoundSlider->setSliderPosition(g_resourceManager->getConfiguration().volumeSound);
	m_volumeSoundSlider->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	addObject(m_volumeSoundSlider);

	distFromTop = distFromTop + 100;

	m_volumeMusicSlider = new Slider(0, 100);
	volumeText = g_textProvider->getText("MusicVolume") + " (%)";
	m_volumeMusicSlider->setTextRaw(volumeText);
	m_volumeMusicSlider->setSliderPosition(g_resourceManager->getConfiguration().volumeMusic);
	m_volumeMusicSlider->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	addObject(m_volumeMusicSlider);

	distFromTop = 150.f;
	distFromLeft = WINDOW_WIDTH / 2.f + 150.f;

	// quickcast
	m_quickCastCheckbox = new Checkbox();
	m_quickCastCheckbox->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	m_quickCastCheckbox->setChecked(g_resourceManager->getConfiguration().isQuickcast);
	m_quickCastCheckbox->setText("Quickcast");
	addObject(m_quickCastCheckbox);

	distFromTop = distFromTop + 50;

	// display hints
	m_displayHintsCheckbox = new Checkbox();
	m_displayHintsCheckbox->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	m_displayHintsCheckbox->setChecked(g_resourceManager->getConfiguration().isDisplayHints);
	m_displayHintsCheckbox->setText("DisplayHints");
	addObject(m_displayHintsCheckbox);

	distFromTop = distFromTop + 50;

	// smoothing
	m_smoothingCheckbox = new Checkbox();
	m_smoothingCheckbox->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	m_smoothingCheckbox->setChecked(g_resourceManager->getConfiguration().isSmoothing);
	m_smoothingCheckbox->setText("Smoothing");
	addObject(m_smoothingCheckbox);

	distFromTop = distFromTop + 50;

	// vsync
	m_vSyncCheckbox = new Checkbox();
	m_vSyncCheckbox->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	m_vSyncCheckbox->setChecked(g_resourceManager->getConfiguration().isVSyncEnabled);
	m_vSyncCheckbox->setText("VSync");
	addObject(m_vSyncCheckbox);

	distFromTop = distFromTop + 50;

	// sound	
	m_soundCheckbox = new Checkbox();
	m_soundCheckbox->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	m_soundCheckbox->setChecked(g_resourceManager->getConfiguration().isSoundOn);
	m_soundCheckbox->setText("Sound");
	addObject(m_soundCheckbox);

	distFromTop = distFromTop + 100;

	// keyboard mappings button
	Button* button = new Button(sf::FloatRect(distFromLeft, distFromTop, 200, 50));
	button->setText("KeyBindings");
	button->setCharacterSize(12);
	button->setOnClick(std::bind(&OptionsScreen::onKeybindings, this));
	addObject(button);

	

	// back
	button = new Button(sf::FloatRect(60, WINDOW_HEIGHT - 100, 200, 50));
	button->setText("Back");
	button->setOnClick(std::bind(&OptionsScreen::onBack, this));
	addObject(button);
	// apply
	button = new Button(sf::FloatRect(WINDOW_WIDTH - 260, WINDOW_HEIGHT - 100, 200, 50));
	button->setText("Apply");
	button->setOnClick(std::bind(&OptionsScreen::onApply, this));
	addObject(button);
}

void OptionsScreen::execOnExit(const Screen *nextScreen) {
	// delete texts (buttons are deleted automatically by the screen)
	delete m_title;
}

void OptionsScreen::onBack() {
	setNextScreen(new MenuScreen(m_characterCore));
}

void OptionsScreen::onApply() {
	ConfigurationData& config = g_resourceManager->getConfiguration();
	bool fullScreenOn = m_displayModeSelector->getChosenOptionIndex() == 1;
	bool fullscreenChanged = config.isFullscreen != fullScreenOn;
	config.language = static_cast<Language>(m_languageSelector->getChosenOptionIndex() + 1);
	config.isSoundOn = m_soundCheckbox->isChecked();
	config.isQuickcast = m_quickCastCheckbox->isChecked();
	config.isDisplayHints = m_displayHintsCheckbox->isChecked();
	config.isFullscreen = fullScreenOn;
	config.isSmoothing = m_smoothingCheckbox->isChecked();
	config.isVSyncEnabled = m_vSyncCheckbox->isChecked();
	config.volumeMusic = m_volumeMusicSlider->getSliderPosition();
	config.volumeSound = m_volumeSoundSlider->getSliderPosition();
	ConfigurationWriter writer;
	writer.saveToFile(config);
	g_textProvider->reload();
	setTooltipText("ConfigurationSaved", COLOR_GOOD, true);
	if (fullscreenChanged) {
		config.isWindowReload = true;
	}
}

void OptionsScreen::onKeybindings() {
	setNextScreen(new KeyBindingsScreen(m_characterCore));
}
