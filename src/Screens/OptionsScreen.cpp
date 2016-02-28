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
	renderTarget.draw(*m_languageText);
	renderTarget.draw(*m_fullscreen);
	renderObjects(GameObjectType::_Button, renderTarget);
	renderTooltipText(renderTarget);
}

void OptionsScreen::execOnEnter(const Screen *previousScreen) {
	// title
	m_title = new BitmapText(g_textProvider->getText("Options"));
	m_title->setCharacterSize(50);
	m_title->setPosition(sf::Vector2f((WINDOW_WIDTH - m_title->getLocalBounds().width) / 2.f, 50.f));

	float distFromTop = 150.f;
	float distFromLeft = 50.f;

	// language
	m_selectedLanguage = g_resourceManager->getConfiguration().language;
	m_languageText = new BitmapText();
	m_languageText->setCharacterSize(12);
	m_languageText->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	refreshLanguageText();

	Button* button;
	distFromTop = distFromTop + 30;
	button = new Button(sf::FloatRect(distFromLeft, distFromTop, 130, 50));
	button->setText("English", 12);
	button->setOnClick(std::bind(&OptionsScreen::onEnglish, this));
	addObject(button);
	button = new Button(sf::FloatRect(distFromLeft + 140, distFromTop, 130, 50));
	button->setText("German", 12);
	button->setOnClick(std::bind(&OptionsScreen::onGerman, this));
	addObject(button);
	button = new Button(sf::FloatRect(distFromLeft + 280, distFromTop, 240, 50));
	button->setText("SwissGerman", 12);
	button->setOnClick(std::bind(&OptionsScreen::onSwiss, this));
	addObject(button);

	distFromTop = distFromTop + 100;

	// displayMode
	m_fullscreen = new BitmapText();
	m_fullscreen->setCharacterSize(12);
	m_fullscreen->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	m_selectedFullscreenOn = g_resourceManager->getConfiguration().isFullscreen;
	refreshFullscreenText();

	distFromTop = distFromTop + 30;
	button = new Button(sf::FloatRect(distFromLeft, distFromTop, 150, 50));
	button->setText("Fullscreen", 12);
	button->setOnClick(std::bind(&OptionsScreen::onFullscreen, this));
	addObject(button);
	button = new Button(sf::FloatRect(distFromLeft + 160, distFromTop, 150, 50));
	button->setText("Window", 12);
	button->setOnClick(std::bind(&OptionsScreen::onWindow, this));
	addObject(button);

	distFromTop = distFromTop + 100;

	// keyboard mappings button
	button = new Button(sf::FloatRect(distFromLeft, distFromTop, 200, 50));
	button->setText("KeyBindings");
	button->setCharacterSize(12);
	button->setOnClick(std::bind(&OptionsScreen::onKeybindings, this));
	addObject(button);

	distFromTop = 150.f;
	distFromLeft = WINDOW_WIDTH / 2.f + 50.f;

	// quickcast
	m_quickCastCheckbox = new Checkbox();
	m_quickCastCheckbox->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	m_quickCastCheckbox->setChecked(g_resourceManager->getConfiguration().isQuickcast);
	m_quickCastCheckbox->setText("Quickcast");
	addObject(m_quickCastCheckbox);

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

void OptionsScreen::refreshLanguageText() {
	string currentLanguage = EnumNames::getLanguageName(m_selectedLanguage);
	string languageText = g_textProvider->getText("Language") + ": ";
	languageText.append(g_textProvider->getText(currentLanguage));
	m_languageText->setString(languageText);
}

void OptionsScreen::refreshFullscreenText() {
	string currentSwitch = m_selectedFullscreenOn ? "Fullscreen" : "Window";
	string fullscreenText = g_textProvider->getText("DisplayMode") + ": ";
	fullscreenText.append(g_textProvider->getText(currentSwitch));
	m_fullscreen->setString(fullscreenText);
}

void OptionsScreen::execOnExit(const Screen *nextScreen) {
	// delete texts (buttons are deleted automatically by the screen)
	delete m_title;
	delete m_languageText;
}

void OptionsScreen::onEnglish() {
	m_selectedLanguage = Language::Lang_EN;
	refreshLanguageText();
}

void OptionsScreen::onSwiss() {
	m_selectedLanguage = Language::Lang_CH;
	refreshLanguageText();
}

void OptionsScreen::onGerman() {
	m_selectedLanguage = Language::Lang_DE;
	refreshLanguageText();
}

void OptionsScreen::onBack() {
	setNextScreen(new MenuScreen(m_characterCore));
}

void OptionsScreen::onApply() {
	ConfigurationData& config = g_resourceManager->getConfiguration();
	bool fullscreenChanged = config.isFullscreen != m_selectedFullscreenOn;
	config.language = m_selectedLanguage;
	config.isSoundOn = m_soundCheckbox->isChecked();
	config.isQuickcast = m_quickCastCheckbox->isChecked();
	config.isFullscreen = m_selectedFullscreenOn;
	config.isSmoothing = m_smoothingCheckbox->isChecked();
	config.isVSyncEnabled = m_vSyncCheckbox->isChecked();
	config.volumeMusic = m_volumeMusicSlider->getSliderPosition();
	config.volumeSound = m_volumeSoundSlider->getSliderPosition();
	ConfigurationWriter writer;
	writer.saveToFile(config);
	g_textProvider->reload();
	setTooltipText("ConfigurationSaved", sf::Color::Green, true);
	if (fullscreenChanged) {
		config.isWindowReload = true;
	}
}

void OptionsScreen::onKeybindings() {
	setNextScreen(new KeyBindingsScreen(m_characterCore));
}

void OptionsScreen::onFullscreen() {
	m_selectedFullscreenOn = true;
	refreshFullscreenText();
}

void OptionsScreen::onWindow() {
	m_selectedFullscreenOn = false;
	refreshFullscreenText();
}
