#pragma once

#include "global.h"
#include "GameObject.h"

#include "GUI/BitmapText.h"
#include "GUI/CircleSector.h"
#include "Structs/SpellBean.h"
#include "ResourceManager.h"
#include "Enums/EnumNames.h"

class SpellSlot : public GameObject
{
public:
	// creates an empty spell slot with type "type"
	SpellSlot(SpellType type = SpellType::VOID);
	// creates a filled spell slot for a spell with id "id"
	SpellSlot(SpellID id);
	// creates a filled spell slot for a spell with bean "bean"
	SpellSlot(const SpellBean& bean);

	void activate();
	void deactivate();
	void select();
	void deselect();

	void highlight(bool highlight);

	void playAnimation(const sf::Time& cooldown);

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	// be aware that the position here is the center of the spell slot. 
	void setPosition(const sf::Vector2f& pos) override;
	void onLeftJustPressed() override;
	void setNr(int nr);

	// asks if it is clicked and sets the member bool to false again (!)
	bool isClicked();

	GameObjectType getConfiguredType() const override;
	SpellType getSpellType() const;
	SpellID getSpellID() const;
	const sf::IntRect& getTextureRect() const;
	int getNr() const;

	static const float RADIUS;

private:
	void init();
	sf::Texture *m_textureInactive = nullptr;
	// the position of the texture icon of this spell.
	sf::IntRect m_textureRect;
	bool m_isChopSlot = false;
	int m_nr = -1;

	bool m_active = false;
	bool m_isClicked = false;
	bool m_isSelected = false;

	sf::Time m_cooldown;
	SpellType m_spellType;
	SpellID m_spellID;
	Key m_inputKeyID;

	sf::Color m_color;
	sf::Color m_colorBase;

	sf::CircleShape m_outerRing;
	sf::CircleShape m_coloredRingBase;
	CircleSector 	m_coloredRing;
	sf::CircleShape m_innerRing;
	sf::CircleShape m_insideBase;
	sf::CircleShape m_inside;

	sf::CircleShape m_smallRingLeft1;
	sf::CircleShape m_smallRingLeft2;
	sf::CircleShape m_smallRingRight1;
	sf::CircleShape m_smallRingRight2;
	sf::CircleShape m_smallRingTop1;
	sf::CircleShape m_smallRingTop2;
	sf::CircleShape m_smallRingBottom1;
	sf::CircleShape m_smallRingBottom2;

	BitmapText m_inputKey;

	bool m_animating = false;
	sf::Time m_animationTime;
};