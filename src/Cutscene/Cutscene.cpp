#include "Cutscene/Cutscene.h"
#include "GUI/GUIConstants.h"
#include "GameObject.h"

const sf::Vector2f Cutscene::TEXT_OFFSET = sf::Vector2f(100.f, 40.f);

Cutscene::Cutscene(std::string& id) {
	m_data = CutsceneLoader::loadCutscene(id);

	m_currentStep = -1;
	if (m_data.id.empty()) return;
	
	m_cutsceneText.setCharacterSize(GUIConstants::CHARACTER_SIZE_L);
	m_cutsceneText.setColor(COLOR_WHITE);
	m_cutsceneText.setTextStyle(TextStyle::Shadowed);
	m_cutsceneText.setTextAlignment(TextAlignment::Center);

	// load all textures now
	for (auto& step : m_data.steps) {
		for (auto& img : step.images) {
			g_resourceManager->getTexture(img.imagePath);
			m_imagePaths.push_back(img.imagePath);
		}
	}

	setNextStep();
}

Cutscene::~Cutscene() {
	for (auto& imagePath : m_imagePaths) {
		g_resourceManager->deleteResource(imagePath);
	}
}

void Cutscene::update(const sf::Time& frameTime) {
	if (g_inputController->isKeyJustPressed(Key::Escape) || g_inputController->isSelected()) {
		m_isNoStepsLeft = true;
		return;
	}

	if (m_fadeInTimer == sf::Time::Zero && m_currentTextTime > sf::Time::Zero) {
		updateTime(m_currentTextTime, frameTime);
	}
	
	if (m_currentTextTime == sf::Time::Zero && m_fadeOutTimer == sf::Time::Zero) {
		setNextText();
		if (m_isNoTextsLeft) {
			setNextStep();
		}
	}

	for (size_t i = 0; i < m_cutsceneImages.size(); ++i) {
		sf::Vector2f oldPos = m_cutsceneImages.at(i).getPosition();
		sf::Vector2f velocity = m_data.steps.at(m_currentStep).images.at(i).velocity;
		m_cutsceneImages.at(i).setPosition(oldPos + velocity * frameTime.asSeconds());
	}

	float scale = 1.f;
	if (m_fadeInTimer > sf::Time::Zero) {
		updateTime(m_fadeInTimer, frameTime);
		scale = 1.f - m_fadeInTimer.asSeconds() / m_fadeTime.asSeconds();
	}
	else if (m_currentTextTime == sf::Time::Zero && m_fadeOutTimer > sf::Time::Zero) {
		updateTime(m_fadeOutTimer, frameTime);
		scale = m_fadeOutTimer.asSeconds() / m_fadeTime.asSeconds();
	}

	const sf::Color& tc = m_cutsceneText.getColor();
	m_cutsceneText.setColor(sf::Color(tc.r, tc.g, tc.b, (sf::Uint8)(scale * 255)));

	for (size_t i = 0; i < m_cutsceneImages.size(); ++i) {
		const sf::Color& tc = m_cutsceneImages.at(i).getColor();
		m_cutsceneImages.at(i).setColor(sf::Color(tc.r, tc.g, tc.b, (sf::Uint8)(scale * 255)));
	}
}

void Cutscene::setNextText() {
	if ((int)m_data.steps.at(m_currentStep).texts.size() <= m_currentText + 1) {
		m_isNoTextsLeft = true;
		return;
	}

	m_currentText++;

	CutsceneStep& step = m_data.steps.at(m_currentStep);
	CutsceneText& text = step.texts.at(m_currentText);

	if (text.text.empty()) {
		m_cutsceneText.setString("");
	}
	else {
		m_cutsceneText.setString(g_textProvider->getCroppedText(text.text, "cutscene",
			GUIConstants::CHARACTER_SIZE_L, WINDOW_WIDTH - 2 * static_cast<int>(TEXT_OFFSET.x)));
	}

	if (text.centered) {
		m_cutsceneText.setCharacterSize(GUIConstants::CHARACTER_SIZE_XXL);
		m_cutsceneText.setPosition(sf::Vector2f(
			0.5f * (WINDOW_WIDTH - m_cutsceneText.getLocalBounds().width), 
			0.5f * (WINDOW_HEIGHT - m_cutsceneText.getLocalBounds().height)));
	}
	else {
		m_cutsceneText.setCharacterSize(GUIConstants::CHARACTER_SIZE_L);
		m_cutsceneText.setPosition(sf::Vector2f(
			0.5f * (WINDOW_WIDTH - m_cutsceneText.getLocalBounds().width), 
			TEXT_OFFSET.y));
	}
	
	
	m_currentTextTime = text.time;
}

void Cutscene::setNextStep() {
	if ((int)m_data.steps.size() <= m_currentStep + 1) {
		m_isNoStepsLeft = true;
		return;
	}

	m_currentStep++;

	CutsceneStep& step = m_data.steps.at(m_currentStep);

	m_currentText = -1;
	m_isNoTextsLeft = false;
	setNextText();

	m_cutsceneImages.clear();

	for (auto& cutsceneImage : step.images) {
		sf::Sprite sprite;
		sprite.setTexture(*g_resourceManager->getTexture(cutsceneImage.imagePath));

		if (cutsceneImage.velocity.x >= 0.f && cutsceneImage.velocity.y >= 0.f) {
			sprite.setPosition(
				WINDOW_WIDTH - sprite.getLocalBounds().width,
				WINDOW_HEIGHT - sprite.getLocalBounds().height);
		}
		else if (cutsceneImage.velocity.x < 0.f && cutsceneImage.velocity.y >= 0.f) {
			sprite.setPosition(0.f, WINDOW_HEIGHT - sprite.getLocalBounds().height);
		}
		else if (cutsceneImage.velocity.x >= 0.f && cutsceneImage.velocity.y < 0.f) {
			sprite.setPosition(WINDOW_WIDTH - sprite.getLocalBounds().width, 0.f);
		}
		else {
			sprite.setPosition(0.f, 0.f);
		}

		m_cutsceneImages.push_back(sprite);
	}

	m_fadeTime = step.fadeTime;
	m_fadeInTimer = m_fadeTime;
	m_fadeOutTimer = m_fadeTime;
}

bool Cutscene::isLoaded() const {
	return m_currentStep > -1;
}

bool Cutscene::isNoStepsLeft() const {
	return m_isNoStepsLeft;
}

const CutsceneData& Cutscene::getData() const {
	return m_data;
}

void Cutscene::render(sf::RenderTarget& target) {
	for (auto& sprite : m_cutsceneImages) {
		target.draw(sprite);
	}
	target.draw(m_cutsceneText);
}
