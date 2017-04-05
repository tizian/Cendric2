#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Structs/SpellModifier.h"
#include "LightObject.h"

class ParticleComponent;

class ModifierTile : public LevelDynamicTile {
public:
	ModifierTile(LevelScreen* levelScreen);

	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void onHit(LevelMovableGameObject* mob) override;

	void setModifier(const SpellModifier& modififer);
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Modifier; }

private:
	void addModifier();
	SpellModifier m_modifier;
	
	void loadComponents();
	ParticleComponent* m_pc;
	sf::Time m_particleTime = sf::seconds(3.f);
};