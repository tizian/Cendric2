#pragma once

#include "global.h"
#include "Level/Enemy.h"
#include "Level/Level.h"
#include "SpellManager.h"
#include "Screen.h"
#include "GUI/SpeechBubble.h"

// will be refactored to talking / scriptable / npc enemy sometime :)
class HunterEnemy : public Enemy {
public:
	HunterEnemy(Level* level, Screen* screen);
	~HunterEnemy() {}

	void loadAnimation() override;
	void update(const sf::Time& frameTime) override;

	sf::Vector2f getConfiguredSpellOffset() const override;
	int getMentalStrength() const override;

	void setDisposed() override;
	void setScreen(Screen* screen) override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override;

protected:
	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	void handleAttackInput();
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;

	SpeechBubble* m_speechBubble;
	int m_speechBubbleState = 0;
};