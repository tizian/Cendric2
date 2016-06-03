#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class InteractComponent;

class CheckpointTile : public LevelDynamicTile {
public:
	CheckpointTile(LevelScreen* levelScreen);
	void init() override;
	void loadAnimation(int skinNr) override; 
	void onRightClick() override;
	void onLeftClick() override;
	void onHit(Spell* spell) override {};
	void onHit(LevelMovableGameObject* mob) override {};

	void setActive(bool active);

private:
	std::string getSpritePath() const override;
	std::string getSoundPath() const override;
	static const float ACTIVATE_RANGE;
	sf::Sound m_sound;
	InteractComponent* m_interactComponent;
};