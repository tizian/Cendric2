#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Structs/SpellModifier.h"
#include "World/LightObject.h"

class ParticleComponent;

class ModifierTile final : public LevelDynamicTile {
public:
	ModifierTile(LevelScreen* levelScreen);

	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void onHit(LevelMovableGameObject* mob) override;

	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Modifier; }

private:
	void addModifier();
	SpellModifier m_modifier;
	
	void loadComponents();
	ParticleComponent* m_pc;
	sf::Time m_particleTime = sf::seconds(3.f);
};