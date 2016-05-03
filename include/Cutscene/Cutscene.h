#pragma once

#include "global.h"

#include "ResourceManager.h"
#include "TextProvider.h"

#include "GUI/BitmapText.h"
#include "Structs/CutsceneData.h"
#include "Cutscene/CutsceneLoader.h"

// a cutscene, displayed in a cutscene screen
class Cutscene {
public:
	Cutscene(std::string& id);
	~Cutscene();

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	bool isLoaded() const;
	bool isNoStepsLeft() const;
	const CutsceneData& getData() const;

private:
	CutsceneData m_data;
	BitmapText m_cutsceneText;
	std::vector<sf::Sprite> m_cutsceneImages;
	std::vector<std::string> m_imagePaths;

	sf::Time m_currentTextTime = sf::Time::Zero;
	int m_currentStep = -1;
	int m_currentText = -1;

	bool m_isNoStepsLeft = false;
	bool m_isNoTextsLeft = false;

	void setNextStep();
	void setNextText();

	// seen from bottom left
	static const sf::Vector2f TEXT_OFFSET;
};