#pragma once

#include "global.h"
#include "Slot.h"

#include "GUI/BitmapText.h"
#include "GUI/CooldownRectangleShape.h"
#include "Structs/SpellData.h"
#include "ResourceManager.h"
#include "Enums/EnumNames.h"

class SpellSlot final : public virtual Slot {
public:
	// creates an empty spell slot with type "type"
	SpellSlot(SpellType type = SpellType::VOID);
	// creates a filled spell slot for a spell with id "id". "Meta" marks a meta slot which holds any spell type.
	SpellSlot(SpellID id, bool isMeta = false);
	// creates a filled spell slot for a spell with data "data"
	SpellSlot(const SpellData& data);

	~SpellSlot();

	void setPosition(const sf::Vector2f& pos) override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	void playAnimation(const sf::Time& cooldown);

	void select() override;
	void deselect() override;

	void setLocked(bool isLocked);
	bool isLocked() const;

	inline void setNr(int nr) { m_nr = nr; }
	
	inline SpellType getSpellType() const { return m_spellType; }
	inline SpellID getSpellID() const { return m_spellID; }
	inline int getNr() const { return m_nr; }
	inline Key getKey() const { return m_inputKeyID; }

	inline float getConfiguredSize() const override { return SIZE; }
	inline float getConfiguredIconOffset() const override { return ICON_OFFSET; }

	static const float SIZE;
	static const float ICON_OFFSET;

private:
	static const float GEM_SIZE;
	void initSpellSlot();

	bool m_isChopSlot = false;
	bool m_isLocked = false;
	int m_nr = -1;

	sf::Time m_cooldown;

	SpellType m_spellType;
	SpellID m_spellID;
	Key m_inputKeyID;

	CooldownRectangleShape m_cooldownRect;
	sf::RectangleShape m_lockedRect;
	BitmapText m_inputKey;
	std::vector<sf::RectangleShape> m_gems;

	bool m_animating = false;
	sf::Time m_animationTime;
};