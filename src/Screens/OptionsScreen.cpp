#include "Screens/OptionsScreen.h"
#include "Screens/MenuScreen.h"
#include "Screens/KeyBindingsScreen.h"

using namespace std;

OptionsScreen::OptionsScreen(CharacterCore* core) : Screen(core)
{
}

Screen* OptionsScreen::update(const sf::Time& frameTime)
{
	if (g_inputController->isKeyActive(Key::Escape) || m_backButton->isClicked())
	{
		return new MenuScreen(m_characterCore);
	}
	else if (m_keyBindingsButton->isClicked())
	{
		return new KeyBindingsScreen(m_characterCore);
	}
	else if (m_applyButton->isClicked())
	{
		g_resourceManager->getConfiguration().language = m_selectedLanguage;
		g_resourceManager->getConfiguration().isSoundOn = m_selectedSoundOn;
		g_resourceManager->getConfiguration().isQuickcast = m_selectedQuickcastOn;
		g_resourceManager->getConfiguration().isFullscreen = m_selectedFullscreenOn;
		g_resourceManager->getConfiguration().isSmoothing = m_selectedSmoothingOn;
		ConfigurationWriter writer;
		writer.saveToFile(g_resourceManager->getConfiguration());
		g_textProvider->reload();
		setTooltipText(g_textProvider->getText("ConfigurationSaved"), sf::Color::Green, true);
	}
	else if (m_englishButton->isClicked())
	{
		m_selectedLanguage = Language::Lang_EN;
		refreshLanguageText();
	}
	else if (m_germanButton->isClicked())
	{

		m_selectedLanguage = Language::Lang_DE;
		refreshLanguageText();

	}
	else if (m_swissButton->isClicked())
	{
		m_selectedLanguage = Language::Lang_CH;
		refreshLanguageText();
	}
	else if (m_soundOnButton->isClicked())
	{
		m_selectedSoundOn = true;
		refreshSoundText();
	}
	else if (m_soundOffButton->isClicked())
	{
		m_selectedSoundOn = false;
		refreshSoundText();
	}
	else if (m_quickcastOnButton->isClicked())
	{
		m_selectedQuickcastOn = true;
		refreshQuickcastText();
	}
	else if (m_quickcastOffButton->isClicked())
	{
		m_selectedQuickcastOn = false;
		refreshQuickcastText();
	}
	else if (m_fullscreenButton->isClicked())
	{
		m_selectedFullscreenOn = true;
		refreshFullscreenText();
	}
	else if (m_windowButton->isClicked())
	{
		m_selectedFullscreenOn = false;
		refreshFullscreenText();
	}
	else if (m_smoothingOnButton->isClicked())
	{
		m_selectedSmoothingOn = true;
		refreshSmoothingText();
	}
	else if (m_smoothingOffButton->isClicked())
	{
		m_selectedSmoothingOn = false;
		refreshSmoothingText();
	}
	updateObjects(GameObjectType::_Button, frameTime);
	updateTooltipText(frameTime);
	deleteDisposedObjects();
	return this;
}

void OptionsScreen::render(sf::RenderTarget &renderTarget)
{
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(*m_title);
	renderTarget.draw(*m_languageText);
	renderTarget.draw(*m_sound);
	renderTarget.draw(*m_quickcast);
	renderTarget.draw(*m_fps);
	renderTarget.draw(*m_volume);
	renderTarget.draw(*m_fullscreen);
	renderTarget.draw(*m_smoothing);
	renderObjects(GameObjectType::_Button, renderTarget);
	renderTooltipText(renderTarget);
}

void OptionsScreen::execOnEnter(const Screen *previousScreen)
{
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
	// quickcast
	m_quickcast = new BitmapText();
	m_quickcast->setCharacterSize(12);
	m_quickcast->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	m_selectedQuickcastOn = g_resourceManager->getConfiguration().isQuickcast;
	refreshQuickcastText();

	distFromTop = distFromTop + 30;
	m_quickcastOnButton = new Button(sf::FloatRect(distFromLeft, distFromTop, 70, 50));
	m_quickcastOnButton->setText("On", 12);
	addObject(m_quickcastOnButton);
	m_quickcastOffButton = new Button(sf::FloatRect(distFromLeft + 80, distFromTop, 70, 50));
	m_quickcastOffButton->setText("Off", 12);
	addObject(m_quickcastOffButton);

	distFromTop = distFromTop + 100;
	// sound
	m_sound = new BitmapText();
	m_sound->setCharacterSize(12);
	m_sound->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	m_selectedSoundOn = g_resourceManager->getConfiguration().isSoundOn;
	refreshSoundText();

	wstring volumeText = g_textProvider->getText("SoundVolume") + L": ";
	volumeText.append(to_wstring(g_resourceManager->getConfiguration().volume));
	volumeText.append(L"%");
	m_volume = new BitmapText(volumeText);
	m_volume->setCharacterSize(12);
	m_volume->setPosition(sf::Vector2f(distFromLeft + 180, distFromTop));

	distFromTop = distFromTop + 30;
	m_soundOnButton = new Button(sf::FloatRect(distFromLeft, distFromTop, 70, 50));
	m_soundOnButton->setText("On", 12);
	addObject(m_soundOnButton);
	m_soundOffButton = new Button(sf::FloatRect(distFromLeft + 80, distFromTop, 70, 50));
	m_soundOffButton->setText("Off", 12);
	addObject(m_soundOffButton);

	distFromTop = distFromTop + 100;
	
	// fps
	wstring fpsText = g_textProvider->getText("MaxFPS") + L": ";
	fpsText.append(to_wstring(g_resourceManager->getConfiguration().maxFrameRate));
	m_fps = new BitmapText(fpsText);
	m_fps->setCharacterSize(12);
	m_fps->setPosition(sf::Vector2f(distFromLeft, distFromTop));

	distFromTop = 150.f;
	distFromLeft = WINDOW_WIDTH / 2.f + 50.f;

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

	// smoothing
	m_smoothing = new BitmapText();
	m_smoothing->setCharacterSize(12);
	m_smoothing->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	m_selectedSmoothingOn = g_resourceManager->getConfiguration().isSmoothing;
	refreshSmoothingText();

	distFromTop = distFromTop + 30;
	m_smoothingOnButton = new Button(sf::FloatRect(distFromLeft, distFromTop, 70, 50));
	m_smoothingOnButton->setText("On", 12);
	addObject(m_smoothingOnButton);
	m_smoothingOffButton = new Button(sf::FloatRect(distFromLeft + 80, distFromTop, 70, 50));
	m_smoothingOffButton->setText("Off", 12);
	addObject(m_smoothingOffButton);

	distFromTop = distFromTop + 100;

	// keyboard mappings button
	m_keyBindingsButton = new Button(sf::FloatRect(WINDOW_WIDTH - 260, distFromTop, 200, 50));
	m_keyBindingsButton->setText("KeyBindings");
	m_keyBindingsButton->setCharacterSize(12);
	addObject(m_keyBindingsButton);
	
	// back
	m_backButton = new Button(sf::FloatRect(60, WINDOW_HEIGHT - 100, 200, 50));
	m_backButton->setText("Back");
	addObject(m_backButton);
	// apply
	m_applyButton = new Button(sf::FloatRect(WINDOW_WIDTH - 260, WINDOW_HEIGHT - 100, 200, 50));
	m_applyButton->setText("Apply");
	addObject(m_applyButton);
}

void OptionsScreen::refreshLanguageText()
{
	string currentLanguage = EnumNames::getLanguageName(m_selectedLanguage);
	wstring languageText = g_textProvider->getText("Language") + L": ";
	languageText.append(g_textProvider->getText(currentLanguage));
	m_languageText->setString(languageText);
}

void OptionsScreen::refreshSoundText()
{
	string currentSwitch = m_selectedSoundOn ? "On" : "Off";
	wstring soundText = g_textProvider->getText("Sound") + L": ";
	soundText.append(g_textProvider->getText(currentSwitch));
	m_sound->setString(soundText);
}

void OptionsScreen::refreshQuickcastText()
{
	string currentSwitch = m_selectedQuickcastOn ? "On" : "Off";
	wstring quickcastText = g_textProvider->getText("Quickcast") + L": ";
	quickcastText.append(g_textProvider->getText(currentSwitch));
	m_quickcast->setString(quickcastText);
}

void OptionsScreen::refreshFullscreenText()
{
	string currentSwitch = m_selectedFullscreenOn ? "Fullscreen" : "Window";
	wstring fullscreenText = g_textProvider->getText("DisplayMode") + L": ";
	fullscreenText.append(g_textProvider->getText(currentSwitch));
	m_fullscreen->setString(fullscreenText);
}

void OptionsScreen::refreshSmoothingText()
{
	string currentSwitch = m_selectedSmoothingOn ? "On" : "Off";
	wstring smoothingText = g_textProvider->getText("Smoothing") + L": ";
	smoothingText.append(g_textProvider->getText(currentSwitch));
	m_smoothing->setString(smoothingText);
}

void OptionsScreen::execOnExit(const Screen *nextScreen)
{
	// delete texts (buttons are deleted automatically by the screen)
	delete m_title;
	delete m_languageText;
	delete m_fps;
	delete m_volume;
	delete m_sound;
	delete m_quickcast;
}