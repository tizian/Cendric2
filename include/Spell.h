#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "ResourceManager.h"
#include "InputController.h"
#include "Level.h"

class MainCharacter;

enum class SpellID
{
	Chop,
	Fire,
	Ice,
	Forcefield
};

/* unlike the values loaded in load() these values here are modifiable by crystal modifiers */
struct SpellBean
{
	int damage;
	int reflectCount;
	sf::Time cooldown;
	sf::Time maxActiveTime;
	float startVelocity;
	sf::FloatRect boundingBox; // gets only set after load if it is not [0,0,0,0]
};

// A spell cendric can cast
class Spell : public MovableGameObject
{
public:

	// called by the spell manager
	void init(SpellBean& bean);
	void loadSpell(Level* level, MainCharacter* mainChar);
	virtual void update(const sf::Time& frameTime) override;
	void checkCollisions(const sf::Vector2f& nextPosition) override;

	// the offset of the spells start position, as seen from the upper mid of cendrics bounding box. The default is the position of the staff head
	virtual const sf::Vector2f getConfiguredPositionOffset() const;
	// returns whether the spell is bound to the main character and will update its position according to the main character. default is false.
	// if this value is set to true, the velocity of a spell has no influence anymore.
	virtual bool getConfiguredIsAttachedToMainChar() const;
	// if true, cendric will be "fighting" as long as the fight animation is. default is false.
	virtual bool getConfiguredTriggerFightAnimation() const;
	virtual SpellID getConfiguredSpellID() const = 0;
	GameObjectType getConfiguredType() const override;

	int getDamage();
	sf::Time getActiveTime();

private:
	Level* m_level;
	MainCharacter* m_mainChar;
	sf::Time m_activeCoolDown;

	int m_damage;
	int m_reflectCount;
	float m_speed;

	// calculates position according to m_mainChar.
	void calculatePositionAccordingToMainChar(sf::Vector2f& position) const;
};