#include "Screens/OptionsScreen.h"
#include "Screens/MenuScreen.h"
#include "Screens/KeyBindingsScreen.h"

using namespace std;

OptionsScreen::OptionsScreen(CharacterCore* core) : Screen(core) {
}

Screen* OptionsScreen::update(const sf::Time& frameTime) {
	if (g_inputController->isKeyActive(Key::Escape) || m_backButton->isClicked()) {
		return new MenuScreen(m_characterCore);
	}
	else if (m_keyBindingsButton->isClicked()) {
		return new KeyBindingsScreen(m_characterCore);
	}
	else if (m_applyButton->isClicked()) {
		g_resourceManager->getConfiguration().language = m_selectedLanguage;
		g_resourceManager->getConfiguration().isSoundOn = m_soundCheckbox->isChecked();
		g_resourceManager->getConfiguration().isQuickcast = m_quickCastCheckbox->isChecked();
		g_resourceManager->getConfiguration().isFullscreen = m_selectedFullscreenOn;
		g_resourceManager->getConfiguration().isSmoothing = m_smoothingCheckbox->isChecked();
		g_resourceManager->getConfiguration().isVSyncEnabled = m_vSyncCheckbox->isChecked();
		g_resourceManager->getConfiguration().volume = m_volumeSlider->getSliderPosition();
		ConfigurationWriter writer;
		writer.saveToFile(g_resourceManager->getConfiguration());
		g_textProvider->reload();
		setTooltipText(g_textProvider->getText("ConfigurationSaved"), sf::Color::Green, true);
	}
	else if (m_englishButton->isClicked()) {
		m_selectedLanguage = Language::Lang_EN;
		refreshLanguageText();
	}
	else if (m_germanButton->isClicked()) {
		m_selectedLanguage = Language::Lang_DE;
		refreshLanguageText();
	}
	else if (m_swissButton->isClicked()) {
		m_selectedLanguage = Language::Lang_CH;
		refreshLanguageText();
	}
	else if (m_fullscreenButton->isClicked()) {
		m_selectedFullscreenOn = true;
		refreshFullscreenText();
	}
	else if (m_windowButton->isClicked()) {
		m_selectedFullscreenOn = false;
		refreshFullscreenText();
	}
	updateObjects(GameObjectType::_Button, frameTime);
	updateTooltipText(frameTime);
	deleteDisposedObjects();
	return this;
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

	distFromTop = distFromTop + 30;
	m_englishButton = new Button(sf::FloatRect(distFromLeft, distFromTop, 130, 50));
	m_englishButton->setText("English", 12);
	addObject(m_englishButton);
	m_germanButton = new Button(sf::FloatRect(distFromLeft + 140, distFromTop, 130, 50));
	m_germanButton->setText("German", 12);
	addObject(m_germanButton);
	m_swissButton = new Button(sf::FloatRect(distFromLeft + 280, distFromTop, 240, 50));
	m_swissButton->setText("SwissGerman", 12);
	addObject(m_swissButton);

	distFromTop = distFromTop + 100;

	// displayMode
	m_fullscreen = new BitmapText();
	m_fullscreen->setCharacterSize(12);
	m_fullscreen->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	m_selectedFullscreenOn = g_resourceManager->getConfiguration().isFullscreen;
	refreshFullscreenText();

	distFromTop = distFromTop + 30;
	m_fullscreenButton = new Button(sf::FloatRect(distFromLeft, distFromTop, 150, 50));
	m_fullscreenButton->setText("Fullscreen", 12);
	addObject(m_fullscreenButton);
	m_windowButton = new Button(sf::FloatRect(distFromLeft + 160, distFromTop, 150, 50));
	m_windowButton->setText("Window", 12);
	addObject(m_windowButton);

	distFromTop = distFromTop + 100;

	// keyboard mappings button
	m_keyBindingsButton = new Button(sf::FloatRect(distFromLeft, distFromTop, 200, 50));
	m_keyBindingsButton->setText("KeyBindings");
	m_keyBindingsButton->setCharacterSize(12);
	addObject(m_keyBindingsButton);

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

	m_volumeSlider = new Slider(0, 100);
	string volumeText = g_textProvider->getText("SoundVolume") + " (%)";
	m_volumeSlider->setTextRaw(volumeText);
	m_volumeSlider->setSliderPosition(g_resourceManager->getConfiguration().volume);
	m_volumeSlider->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	addObject(m_volumeSlider);

	// back
	m_backButton = new Button(sf::FloatRect(60, WINDOW_HEIGHT - 100, 200, 50));
	m_backButton->setText("Back");
	addObject(m_backButton);
	// apply
	m_applyButton = new Button(sf::FloatRect(WINDOW_WIDTH - 260, WINDOW_HEIGHT - 100, 200, 50));
	m_applyButton->setText("Apply");
	addObject(m_applyButton);
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