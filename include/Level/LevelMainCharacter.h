#pragma once

#include "global.h"
#include "LevelMovableGameObject.h"
#include "Level.h"
#include "InputController.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"
#include "CharacterCore.h"
#include "MainCharacter.h"
#include "TargetManager.h"

// Cendric in a level
class LevelMainCharacter : public virtual LevelMovableGameObject, public virtual MainCharacter {
	friend class UserMovingBehavior;
public:
	LevelMainCharacter(Level* level);
	~LevelMainCharacter();

	void load();
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;
	void onHit(Spell* spell) override;

	void updateFirst(const sf::Time& frameTime) override { LevelMovableGameObject::updateFirst(frameTime); }
	void renderAfterForeground(sf::RenderTarget& target) override { LevelMovableGameObject::renderAfterForeground(target); }
	void setDebugBoundingBox(const sf::Color& color) override { LevelMovableGameObject::setDebugBoundingBox(color); }
	void setState(GameObjectState state) override { LevelMovableGameObject::setState(state); }
	void setState(GameObjectState state, bool updateAnimation) override { LevelMovableGameObject::setState(state, updateAnimation); }

	MovingBehavior* createMovingBehavior(bool asAlly = false) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly = false) override;

	void setPosition(const sf::Vector2f& pos) override;
	void setCharacterCore(CharacterCore* core);
	void setInvisibilityLevel(int level);
	void addDamage(int damage, DamageType damageType, bool overTime, bool critical) override;
	void addDamageOverTime(DamageOverTimeData& data) override;
	void setFeared(const sf::Time& fearedTime) override;
	void setStunned(const sf::Time& stunnedTime) override;
	void setDead() override;
	void setQuickcast(bool quickcast);
	void setGodmode(bool godmode);
	// adds the item to the players inventory
	void lootItem(const std::string& item, int quantity) const;
	void lootItems(std::map<std::string, int>& items) const;
	void addGold(int gold) const;
	void removeGold(int gold) const;
	void removeItems(const std::string& item, int quantity) const;

	bool isAlly() const override;
	bool isClimbing() const;

	// ranges from 0 to 4 and helps render the main char invisibile for certain enemies / reduce the aggro range
	int getInvisibilityLevel() const;

	GameObjectType getConfiguredType() const override;
	TargetManager& getTargetManager() const;

protected:
	std::string getSpritePath() const override;
	std::string getDeathSoundPath() const override;

private:
	void loadWeapon();		// character core must be set when loading the weapon.
	void loadAnimation();

	void handleAttackInput();

	void loadParticleSystem();
	void updateDamagedOverlay();
	int getSpellFromKey(Key key);

private:
	CharacterCore* m_core;
	ScreenOverlay* m_damagedScreenOverlay = nullptr;
	TargetManager* m_targetManager;
	
	std::map<Key, int> m_spellKeyMap;
	bool m_isQuickcast;
	int m_invisibilityLevel = 0;

	sf::Time m_fadingTime = sf::seconds(2.f);
	sf::Time m_particleTime = sf::seconds(2.f);

	particles::TextureParticleSystem* m_ps = nullptr;
	particles::ParticleSpawner* m_particleSpawner = nullptr;
};