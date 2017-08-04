#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

enum class SwingingTileMode {
	Round,
	Ease
};

class SwingingTile final : public LevelDynamicTile, public LeverDependentTile {
public:
	SwingingTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	~SwingingTile();
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void update(const sf::Time& frametime) override;
	void render(sf::RenderTarget& target) override;
	void onHit(LevelMovableGameObject* mob) override;
	void onHit(Spell* spell) override { /*nop*/ }
	void setPosition(const sf::Vector2f& position) override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Swinging; }

	void switchTile() override;
	bool isSwitchable() const override;

private:
	void setInitialState(bool on) override;
	std::string getSpritePath() const override;

private:
	int m_size;
	int m_speed;
	bool m_isInactive = false;
	SwingingTileMode m_mode;
	sf::Texture* m_texture = nullptr;
	float m_currentRotation = 0.f;
};