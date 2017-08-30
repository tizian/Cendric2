#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Structs/SpellModifier.h"
#include "World/LightObject.h"

class ParticleComponent;

class SpellTile final : public LevelDynamicTile {
public:
	SpellTile(LevelScreen* levelScreen);

	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override {}
	void onHit(LevelMovableGameObject* mob) override;

	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Spell; }

protected:
	std::string getSpritePath() const override;

private:
	void touch();
	void addSpell();
	void loadLeapOfFaithSpell();
	void loadFlashSpell();
	void loadGhostFormSpell(); 
	SpellID m_spellID;
	
	void loadComponents();
	ParticleComponent* m_pc;
	sf::Time m_particleTime = sf::seconds(3.f);

	static const std::string THUNDER_SOUND_PATH;
};