#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class LevelMainCharacter;
class InteractComponent;

namespace particles {
	class LineSpawner;
	class ParticleSystem;
	class AngledVelocityGenerator;
}

class MirrorTile final : public LevelDynamicTile {
public:
	explicit MirrorTile(LevelScreen* levelScreen);
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void onRightClick() override;
	void setRotation(float rotation);

	void switchLever();
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Mirror; }

private:
	std::string getSpritePath() const override;

private:
	// the mirror lever can only be used by hand if the main char is in this range.
	// it is however always possible for a telekinesis spell to activate the lever if its projectile reaches the lever.
	static const float ACTIVATE_RANGE;
	static const float TICK_ANGLE;

	InteractComponent* m_interactComponent;
	float m_currentRotation;
};

class MirrorRay final : public GameObject {
public:
	explicit MirrorRay(LevelScreen* levelScreen);
	~MirrorRay() {};

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	GameObjectType getConfiguredType() const override { return GameObjectType::_Undefined; }

private:
	void recalculateRays();

private:
	LevelScreen* m_screen;

	std::vector<particles::ParticleSystem*> m_rays;
	std::vector<particles::LineSpawner*> m_lineSpawners;
	std::vector<particles::AngledVelocityGenerator*> m_lineVelGens;
};