#pragma once

#include "global.h"
#include "Slot.h"

#include "GUI/BitmapText.h"
#include "GUI/CooldownRectangleShape.h"
#include "Structs/SpellData.h"
#include "ResourceManager.h"
#include "Enums/EnumNames.h"

class SpellSlot : public Slot {
public:
	// creates an empty spell slot with type "type"
	SpellSlot(SpellType type = SpellType::VOID);
	// creates a filled spell slot for a spell with id "id"
	SpellSlot(SpellID id);
	// creates a filled spell slot for a spell with data "data"
	SpellSlot(const SpellData& data);

	void setPosition(const sf::Vector2f& pos) override;

	void render(sf::RenderTarget& renderTarget) override;

	void update(const sf::Time& frameTime) override;

	void playAnimation(const sf::Time& cooldown);

	void select() override;
	void deselect() override;

	inline void setNr(int nr) { m_nr = nr; }
	
	inline SpellType getSpellType() const { return m_spellType; }
	inline SpellID getSpellID() const { return m_spellID; }
	inline int getNr() const { return m_nr; }

	inline float getConfiguredSize() const override { return SIZE; }
	inline float getConfiguredIconOffset() const override { return ICON_OFFSET; }

	static const float SIZE;
	static const float ICON_OFFSET;

private:
	void initSpellSlot();

	bool m_isChopSlot = false;
	int m_nr = -1;

	sf::Time m_cooldown;

	SpellType m_spellType;
	SpellID m_spellID;
	Key m_inputKeyID;

	CooldownRectangleShape m_cooldownRect;
	BitmapText m_inputKey;
	sf::RectangleShape m_gemsRect;

	bool m_animating = false;
	sf::Time m_animationTime;
};