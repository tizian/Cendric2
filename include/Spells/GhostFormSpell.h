#pragma once

#include "global.h"
#include "Spell.h"

class LevelEquipment;

class GhostFormSpell : public Spell {
public:
	GhostFormSpell(const AttributeData& additionalDamage);
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;
	void execOnHit(LevelMovableGameObject* target) override;

	sf::Vector2f getConfiguredPositionOffset() const override;
	bool getConfiguredRotateSprite() const override;

	void setDisposed() override;

private:
	LevelEquipment* m_mask = nullptr;
	float m_velocityScale;
	static const sf::Color GHOST_COLOR;

	// when this spell fades, the owner is taken to the last safe (non-colliding) position
	sf::Vector2f m_lastSafePosition;

	std::unique_ptr<particles::TextureParticleSystem> m_ps;
	particles::AngledVelocityGenerator* m_velGenerator;
	particles::BoxPositionGenerator* m_posGenerator;

	void loadParticleSystem(float startVelocity);
	void loadMask();

	void updateParticleSystemPosition();

	AttributeData m_additionalDamage;
};