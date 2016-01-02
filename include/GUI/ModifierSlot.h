#pragma once

#include "global.h"
#include "Slot.h"
#include "Structs/SpellModifier.h"

class CharacterCore;
class ModifierDescriptionWindow;

class ModifierSlot : public Slot {
public:
	ModifierSlot(const SpellModifier& modifier);

	~ModifierSlot();

	void setPosition(const sf::Vector2f& pos) override;

	void renderAfterForeground(sf::RenderTarget& renderTarget) override;

	virtual void onMouseOver() override;

	const SpellModifier& getModifier() const { return m_spellModifier; }
	inline int getSpellSlotNr() const { return m_spellSlotNr; }
	inline int getNr() const { return m_nr; }

	inline void setSpellSlotNr(int nr) { m_spellSlotNr = nr; }
	inline void setNr(int nr) { m_nr = nr; }

	inline float getConfiguredSize() const override { return SIZE; }
	inline float getConfiguredIconOffset() const override { return ICON_OFFSET; }

	static const float SIZE;
	static const float ICON_OFFSET;

private:
	SpellModifier m_spellModifier;

	int m_nr = -1;
	int m_spellSlotNr = -1;

	bool m_showDescriptionWindow = false;
	ModifierDescriptionWindow* m_descriptionWindow = nullptr;
};