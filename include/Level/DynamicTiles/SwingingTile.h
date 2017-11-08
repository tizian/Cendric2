#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Level/DynamicTiles/LeverDependentTile.h"

enum class SwingingTileMode {
	Round,
	Pendulum,
};

class SwingingTile final : public LevelDynamicTile, public LeverDependentTile {
public:
	SwingingTile(LevelScreen* levelScreen);
	~SwingingTile();
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void update(const sf::Time& frametime) override;
	void renderAfterForeground(sf::RenderTarget& target) override;
	void onHit(LevelMovableGameObject* mob) override;
	void onHit(Spell* spell) override { /*nop*/ }
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Swinging; }

	void switchTile() override;
	bool isSwitchable() const override;

private:
	void setInitialState(bool on) override;
	std::string getSpritePath() const override;
	sf::Vector2f getHeadPosition() const;

private:
	void animatePendulum(const sf::Time& frametime);
	void animateRound(const sf::Time& frametime);

private:
	int m_size;
	int m_speed;
	bool m_isInactive = false;
	bool m_isClockwise = true;
	float m_pendulumVelocity = 0.f;
	float m_length;
	SwingingTileMode m_mode;
	sf::Texture* m_texture = nullptr;
	float m_currentRotation = 0.f;
	sf::CircleShape m_debugCircle;
	static const float DAMAGE_RADIUS;
};