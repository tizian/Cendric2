#pragma once

#include "global.h"
#include "Spells/Spell.h"

class LevelEquipment;
class ParticleComponent;

class GhostFormSpell final : public Spell {
public:
	GhostFormSpell(const AttributeData& additionalDamage);
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;
	void execOnHit(LevelMovableGameObject* target) override;

	bool getConfiguredRotateSprite() const override;

	void setDisposed() override;
	void onOwnerDisposed() override;

private:
	LevelEquipment* m_mask = nullptr;
	static const sf::Color GHOST_COLOR;

	void loadComponents(float startSpeed);
	void loadMask();

	particles::AngledVelocityGenerator* m_velGen;
	void updateParticleSystemPosition();

	AttributeData m_additionalDamage;
};