#include "Cutscene/Cutscene.h"
#include "GUI/GUIConstants.h"
#include "GameObject.h"

const sf::Vector2f Cutscene::TEXT_OFFSET = sf::Vector2f(50.f, 20.f);

Cutscene::Cutscene(std::string& id) {
	m_data = CutsceneLoader::loadCutscene(id);

	m_currentStep = -1;
	if (m_data.id.empty()) return;
	
	m_cutsceneText.setCharacterSize(GUIConstants::CHARACTER_SIZE_L);
	m_cutsceneText.setColor(CENDRIC_COLOR_WHITE);

	setNextStep();
}

Cutscene::~Cutscene() {
	for (auto& imagePath : m_imagePaths) {
		g_resourceManager->deleteResource(imagePath);
	}
}

void Cutscene::update(const sf::Time& frameTime) {
	GameObject::updateTime(m_currentTextTime, frameTime);
	if (m_currentTextTime == sf::Time::Zero) {
		setNextText();
		if (m_isNoTextsLeft) {
			setNextStep();
		}
	}

	for (int i = 0; i < m_cutsceneImages.size(); ++i) {
		sf::Vector2f oldPos = m_cutsceneImages.at(i).getPosition();
		sf::Vector2f velocity = m_data.steps.at(m_currentStep).images.at(i).velocity;
		m_cutsceneImages.at(i).setPosition(oldPos + velocity * frameTime.asSeconds());
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
			GUIConstants::CHARACTER_SIZE_L, WINDOW_WIDTH - 2 * TEXT_OFFSET.x));
	}
	m_cutsceneText.setPosition(sf::Vector2f(
		(WINDOW_WIDTH - m_cutsceneText.getLocalBounds().width) / 2.f, 
		WINDOW_HEIGHT - TEXT_OFFSET.y - m_cutsceneText.getLocalBounds().height));
	
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

	for (auto& imagePath : m_imagePaths) {
		g_resourceManager->deleteResource(imagePath);
	}
	m_cutsceneImages.clear();
	m_imagePaths.clear();
	for (auto& cutsceneImage : step.images) {
		
		m_imagePaths.push_back(cutsceneImage.imagePath);
		sf::Sprite sprite;
		sprite.setTexture(*g_resourceManager->getTexture(cutsceneImage.imagePath));

		if (cutsceneImage.velocity.x >= 0.f && cutsceneImage.velocity.y >= 0.f) {
			sprite.setPosition(0.f, 0.f);
		}
		else if (cutsceneImage.velocity.x < 0.f && cutsceneImage.velocity.y >= 0.f) {
			sprite.setPosition(WINDOW_WIDTH - sprite.getLocalBounds().width, 0.f);
		}
		else if (cutsceneImage.velocity.x >= 0.f && cutsceneImage.velocity.y < 0.f) {
			sprite.setPosition(0.f, WINDOW_HEIGHT - sprite.getLocalBounds().height);
		}
		else {
			sprite.setPosition(
				WINDOW_WIDTH - sprite.getLocalBounds().width, 
				WINDOW_HEIGHT - sprite.getLocalBounds().height);
		}

		m_cutsceneImages.push_back(sprite);
	}
}

bool Cutscene::isLoaded() const {
	return m_currentStep > -1;
}

bool Cutscene::isNoStepsLeft() const {
	return m_isNoStepsLeft;
}

void Cutscene::render(sf::RenderTarget& target) {
	for (auto& sprite : m_cutsceneImages) {
		target.draw(sprite);
	}
	target.draw(m_cutsceneText);
}
