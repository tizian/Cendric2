#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class LevelMainCharacter;
class InteractComponent;

namespace particles {
	class LineSpawner;
	class TextureParticleSystem;
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

class Ray final {
public:
	explicit Ray(const Level* level);
	~Ray();

	// casts the ray using origin and direction and returns the end position
	const sf::Vector2f& cast(const sf::Vector2f& origin, const sf::Vector2f& direction);
	void update(const sf::Time& frameTime);
	void render(sf::RenderTarget& target);

private:
	void loadParticleSystem();

	particles::TextureParticleSystem* m_particleSystem = nullptr;
	particles::LineSpawner* m_lineSpawner = nullptr;
	particles::AngledVelocityGenerator* m_lineVelGen = nullptr;

	sf::Vector2f m_startPos;
	sf::Vector2f m_endPos;
	sf::Vector2f m_direction;

	const Level* m_level;
	float m_currentAngle = 0.f;
};

class MirrorRay final : public GameObject {
public:
	explicit MirrorRay(LevelScreen* levelScreen);
	~MirrorRay();

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void initRay(const sf::Vector2f& origin, const sf::Vector2f& direction);

	GameObjectType getConfiguredType() const override { return GameObjectType::_Undefined; }

private:
	void recalculateRays();
	sf::Time m_recalculateCooldown;

private:
	LevelScreen* m_screen;

	std::vector<Ray*> m_rays;
};