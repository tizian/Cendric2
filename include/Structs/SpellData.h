#pragma once

#include <SFML/Graphics.hpp>
#include "global.h"

#include "Enums/SpellID.h"
#include "Enums/SpellType.h"
#include "Enums/DamageType.h"
#include "Enums/Key.h"
#include "Enums/GameObjectState.h"
#include "Structs/SpellModifier.h"

class SpellCreator;
class LevelMovableGameObject;

/* describes the attributes of a spell */
struct SpellData final {
	SpellID id = SpellID::VOID;
	SpellType spellType = SpellType::VOID;
	std::string spritesheetPath;
	std::string soundPath;
	bool isSoundLooping = false;
	sf::IntRect iconTextureRect;
	sf::Time cooldown = sf::seconds(1.f);
	sf::FloatRect boundingBox;
	sf::Vector2f spellOffset; // as seen from the upper mid of the owners bounding box.
	DamageType damageType = DamageType::VOID;
	GameObjectState castingAnimation = GameObjectState::Casting;
	GameObjectState fightAnimation = GameObjectState::Fighting;
	sf::Time castingTime;
	sf::Time fightingTime = sf::milliseconds(5 * 70); // cendric's default fighting time
	bool needsTarget = false;
	bool attachedToMob = false;
	bool isDynamicTileEffect = false;
	bool isColliding = true;
	bool isStunning = false;
	bool isFearing = false;
	bool isBlocking = false;
	bool isAlly = true; // all spells sent by Cendric and allied mobs are "allied" spells that will hurt non-allied enemies and vice versa.
	int skinNr = 0; // can be used to change the skin of the spell. Must be implemented by the spell itself.
	bool critical = false;

	// modifiable by crystal modifiers
	int damage = 0;
	int damagePerSecond = 0;
	int heal = 0;
	int reflectCount = 0;
	float speed = 0.f;
	int count = 1;
	float range = 0.f;
	int strength = 1; // used for special values, such as wind force or lockpick strength.
	int ccStrength = -1; // used for the isStunning and isFearing variables. (crowd control)
	sf::Time duration; // duration of an effect of a spell, for example the fear duration (fear spell)
	sf::Time activeDuration;	// the duration for which a spell is active before it gets disposed. 
								//This value is not displayed to the user in contrast to the duration.

	float divergenceAngle = 0.f;
	float rangeModifierAddition = 0.f;
	float speedModifierAddition = 0.f;
	int countModifierAddition = 0;
	int reflectModifierAddition = 0;
	sf::Time durationModifierAddition;

	Key inputKey = Key::VOID;

	static SpellData getSpellData(SpellID id);
	static std::vector<SpellModifierType> getAllowedModifiers(SpellID id);
	static SpellCreator* getSpellCreator(const SpellData& data, const std::vector<SpellModifier>& modifiers, LevelMovableGameObject* owner);

private:
	static SpellData getChopSpellData();

	static SpellData getFireBallSpellData();
	static SpellData getIceBallSpellData();
	static SpellData getTelekinesisSpellData();
	static SpellData getWindGustSpellData();
	static SpellData getAntiGravitySpellData();

	static SpellData getUnlockSpellData();
	static SpellData getShadowTrapSpellData();
	static SpellData getInvisibilitySpellData();
	static SpellData getFlashSpellData();
	static SpellData getIcyAmbushSpellData();

	static SpellData getFearSpellData();
	static SpellData getLeechSpellData();
	static SpellData getRaiseTheDeadSpellData();
	static SpellData getGhostFormSpellData();
	static SpellData getSummonGargoyleSpellData();

	static SpellData getLightSpellData();
	static SpellData getHolyFireSpellData();
	static SpellData getDivineShieldSpellData();
	static SpellData getLeapOfFaithSpellData();
	static SpellData getAureolaSpellData();

	static SpellData getProjectileSpellData();
	static SpellData getBuffSpellData();
	static SpellData getReturningProjectileSpellData();
	static SpellData getBoomerangSpellData();
	static SpellData getRotatingProjectileData();
	static SpellData getTargetingProjectileData();
};
