#include "Screens/OptionsScreen.h"
#include "Screens/MenuScreen.h"

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
	else if (m_applyButton->isClicked())
	{
		g_resourceManager->getConfiguration().language = m_selectedLanguage;
		g_resourceManager->getConfiguration().isSoundOn = m_selectedSoundOn;
		ConfigurationWriter writer;
		writer.saveToFile(g_resourceManager->getConfiguration());
		g_textProvider->reload();
		setTooltipText(g_textProvider->getText("ConfigurationSaved"), sf::Color::White, true);
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
	renderTarget.draw(*m_fps);
	renderTarget.draw(*m_volume);
	for (auto it : m_keyTexts)
	{
		renderTarget.draw(*it);
	}
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
	float distFromTopKeymap = 150.f;
	float distFromLeft = 50.f;
	float distFromLeftKeymap = (WINDOW_WIDTH - 460);

	// language
	m_selectedLanguage = g_resourceManager->getConfiguration().language;
	m_languageText = new BitmapText();
	m_languageText->setCharacterSize(12);
	m_languageText->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	refreshLanguageText();

	distFromTop = distFromTop + 30;
	m_englishButton = new Button(sf::FloatRect(distFromLeft, distFromTop, 130, 50));
	m_englishButton->setText("English", 12);
	addObject(GameObjectType::_Button, m_englishButton);
	m_germanButton = new Button(sf::FloatRect(distFromLeft + 140, distFromTop, 130, 50));
	m_germanButton->setText("German", 12);
	addObject(GameObjectType::_Button, m_germanButton);
	m_swissButton = new Button(sf::FloatRect(distFromLeft + 280, distFromTop, 240, 50));
	m_swissButton->setText("SwissGerman", 12);
	addObject(GameObjectType::_Button, m_swissButton);

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
	addObject(GameObjectType::_Button, m_soundOnButton);
	m_soundOffButton = new Button(sf::FloatRect(distFromLeft + 80, distFromTop, 70, 50));
	m_soundOffButton->setText("Off", 12);
	addObject(GameObjectType::_Button, m_soundOffButton);


	distFromTop = distFromTop + 100;
	// fps
	wstring fpsText = g_textProvider->getText("MaxFPS") + L": ";
	fpsText.append(to_wstring(g_resourceManager->getConfiguration().maxFrameRate));
	m_fps = new BitmapText(fpsText);
	m_fps->setCharacterSize(12);
	m_fps->setPosition(sf::Vector2f(distFromLeft, distFromTop));

	// keyboard mappings
	for (auto it : g_resourceManager->getConfiguration().keyMap)
	{
		BitmapText* keyText = new BitmapText(g_textProvider->getText(EnumNames::getKeyName(it.first)));
		keyText->setCharacterSize(12);
		keyText->setPosition(sf::Vector2f(distFromLeftKeymap, distFromTopKeymap));
		m_keyTexts.push_back(keyText);
		Button* keyButton = new Button(sf::FloatRect(distFromLeftKeymap + 300.f, distFromTopKeymap, 100, 20));
		keyButton->setTextRaw(EnumNames::getKeyboardKeyName(it.second), 12);
		addObject(GameObjectType::_Button, keyButton);
		distFromTopKeymap = distFromTopKeymap + 25;
	}

	// back
	m_backButton = new Button(sf::FloatRect(60, WINDOW_HEIGHT - 100, 200, 50));
	m_backButton->setText("Back");
	addObject(GameObjectType::_Button, m_backButton);
	// apply
	m_applyButton = new Button(sf::FloatRect(WINDOW_WIDTH - 260, WINDOW_HEIGHT - 100, 200, 50));
	m_applyButton->setText("Apply");
	addObject(GameObjectType::_Button, m_applyButton);
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

void OptionsScreen::execOnExit(const Screen *nextScreen)
{
	// delete texts (buttons are deleted automatically by the screen)
	delete m_title;
	delete m_languageText;
	delete m_fps;
	delete m_volume;
	delete m_sound;
	for (auto it : m_keyTexts)
	{
		delete it;
	}
	m_keyTexts.clear();
}