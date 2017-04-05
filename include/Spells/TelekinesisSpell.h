#pragma once

#include "global.h"
#include "Spells/Spell.h"

class TelekinesisSpell : public Spell {
public:
	TelekinesisSpell();
	
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	
	void setItemVector(const std::vector<GameObject*>* items);

private:
	void checkCollisionsWithItems();
	void loadComponents();
	const std::vector<GameObject*>* m_items;
};