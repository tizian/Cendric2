#include "GUI/SpellSlot.h"

using namespace std;

const float SpellSlot::RADIUS = 35.f;

SpellSlot::SpellSlot(SpellType type)
{
	m_spellType = type;
	m_spellID = SpellID::VOID;
	m_isChopSlot = false;
	m_inputKeyID = Key::VOID;

	init();
}

SpellSlot::SpellSlot(SpellID id)
{
	m_spellID = id;
	const SpellBean& bean = SpellBean::getSpellBean(id);
	m_textureRect = bean.iconTextureRect;
	m_spellType = bean.spellType;
	m_isChopSlot = (id == SpellID::Chop);
	m_inputKeyID = Key::VOID;

	init();
}

SpellSlot::SpellSlot(const SpellBean& bean)
{
	m_spellID = bean.id;
	m_textureRect = bean.iconTextureRect;
	m_spellType = bean.spellType;
	m_isChopSlot = (bean.id == SpellID::Chop);
	m_inputKeyID = bean.inputKey;

	init();
}

void SpellSlot::init()
{
	setBoundingBox(sf::FloatRect(0.f, 0.f, 2 * RADIUS, 2 * RADIUS));
	setDebugBoundingBox(sf::Color::Red);
	setInputInDefaultView(true);

	m_color = CENDRIC_COLOR_GREY;
	m_colorBase = CENDRIC_COLOR_BLACK;
	if (m_spellType == SpellType::Elemental)
	{
		m_color = CENDRIC_COLOR_ELEMENTAL;
		m_colorBase = CENDRIC_COLOR_ELEMENTAL_INACTIVE;
		m_textureInactive = g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_elemental);
	}
	else if (m_spellType == SpellType::Twilight)
	{
		m_color = CENDRIC_COLOR_TWILIGHT;
		m_colorBase = CENDRIC_COLOR_TWILIGHT_INACTIVE;
		m_textureInactive = g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_twilight);
	}
	else if (m_spellType == SpellType::Necromancy)
	{
		m_color = CENDRIC_COLOR_NECROMANCY;
		m_colorBase = CENDRIC_COLOR_NECROMANCY_INACTIVE;
		m_textureInactive = g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_necromancy);
	}
	else if (m_spellType == SpellType::Divine)
	{
		m_color = CENDRIC_COLOR_DIVINE;
		m_colorBase = CENDRIC_COLOR_DIVINE_INACTIVE;
		m_textureInactive = g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_divine);
	}
	else if (m_spellType == SpellType::Illusion)
	{
		m_color = CENDRIC_COLOR_ILLUSION;
		m_colorBase = CENDRIC_COLOR_ILLUSION_INACTIVE;
		m_textureInactive = g_resourceManager->getTexture(ResourceID::Texture_GUI_spell_color_illusion);
	}

	m_inputKey.setString(m_inputKeyID != Key::VOID ?
		EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap[m_inputKeyID]) :
		"");
	m_inputKey.setCharacterSize(16);
	if (m_inputKey.getLocalBounds().width > 2 * RADIUS - 10.f) m_inputKey.setCharacterSize(8);
	m_inputKey.setColor(CENDRIC_COLOR_WHITE);

	m_outerRing = sf::CircleShape(RADIUS);
	m_outerRing.setFillColor(CENDRIC_COLOR_WHITE);

	m_coloredRingBase = sf::CircleShape(0.95f * RADIUS);
	m_coloredRingBase.setFillColor(m_colorBase);

	m_coloredRing = CircleSector(0.95f * RADIUS);
	m_coloredRing.setFillColor(m_color);

	m_innerRing = sf::CircleShape(0.75f * RADIUS);
	m_innerRing.setFillColor(CENDRIC_COLOR_WHITE);

	m_insideBase = sf::CircleShape(0.7f * RADIUS);
	m_insideBase.setFillColor(CENDRIC_COLOR_LIGHT_GREY);

	m_inside = sf::CircleShape(0.7f * RADIUS);

	m_smallRingLeft1 = sf::CircleShape(0.25f * RADIUS);
	m_smallRingLeft1.setOutlineColor(CENDRIC_COLOR_WHITE);
	m_smallRingLeft1.setOutlineThickness(0.05f * RADIUS);

	m_smallRingLeft2 = sf::CircleShape(0.05f * RADIUS);
	m_smallRingLeft2.setOutlineColor(CENDRIC_COLOR_WHITE);
	m_smallRingLeft2.setOutlineThickness(0.05f * RADIUS);

	m_smallRingRight1 = m_smallRingLeft1;
	m_smallRingRight2 = m_smallRingLeft2;

	m_smallRingTop1 = m_smallRingLeft1;
	m_smallRingTop2 = m_smallRingLeft2;

	m_smallRingBottom1 = m_smallRingLeft1;
	m_smallRingBottom2 = m_smallRingLeft2;

	if (m_spellID != SpellID::VOID)
	{
		activate();
	}
	else
	{
		deactivate();
	}
}

void SpellSlot::select()
{
	if (m_isSelected) return;
	m_isSelected = true;
	m_smallRingBottom1.setFillColor(sf::Color::Red);
	m_smallRingTop1.setFillColor(sf::Color::Red);
	m_smallRingLeft1.setFillColor(sf::Color::Red);
	m_smallRingRight1.setFillColor(sf::Color::Red);
}

void SpellSlot::highlight(bool highlight)
{
	if (highlight)
	{
		m_smallRingBottom1.setFillColor(sf::Color::Green);
		m_smallRingTop1.setFillColor(sf::Color::Green);
		m_smallRingLeft1.setFillColor(sf::Color::Green);
		m_smallRingRight1.setFillColor(sf::Color::Green);
	}
	else
	{
		m_smallRingBottom1.setFillColor(m_isSelected ? sf::Color::Red : m_active ? m_color : m_colorBase);
		m_smallRingTop1.setFillColor(m_isSelected ? sf::Color::Red : m_active ? m_color : m_colorBase);
		m_smallRingLeft1.setFillColor(m_isSelected ? sf::Color::Red : m_active ? m_color : m_colorBase);
		m_smallRingRight1.setFillColor(m_isSelected ? sf::Color::Red : m_active ? m_color : m_colorBase);
	}
}

void SpellSlot::deselect()
{
	if (!m_isSelected) return;
	m_isSelected = false;
	m_smallRingBottom1.setFillColor(m_color);
	m_smallRingTop1.setFillColor(m_color);
	m_smallRingLeft1.setFillColor(m_color);
	m_smallRingRight1.setFillColor(m_color);
}

bool SpellSlot::isClicked()
{
	return m_isClicked;
}

bool SpellSlot::isRightClicked()
{
	return m_isRightClicked;
}

void SpellSlot::setPosition(const sf::Vector2f& pos)
{
	m_position = pos;
	m_boundingBox.left = pos.x - RADIUS;
	m_boundingBox.top = pos.y - RADIUS;

	sf::Vector2f radiusVector(RADIUS, RADIUS);
	m_debugBox.setPosition(pos - radiusVector);

	m_inputKey.setPosition(pos - sf::Vector2f(radiusVector));
	m_outerRing.setPosition(pos - sf::Vector2f(radiusVector));
	m_coloredRingBase.setPosition(pos - 0.95f * radiusVector);
	m_coloredRing.setPosition(pos - 0.95f * radiusVector);
	m_innerRing.setPosition(pos - 0.75f * radiusVector);
	m_insideBase.setPosition(pos - 0.7f * radiusVector);
	m_inside.setPosition(pos - 0.7f * radiusVector);

	m_smallRingLeft1.setPosition(pos - sf::Vector2f(0.85f * RADIUS, 0.f) - 0.25f * radiusVector);
	m_smallRingLeft2.setPosition(pos - sf::Vector2f(0.85f * RADIUS, 0.f) - 0.05f * radiusVector);
	m_smallRingRight1.setPosition(pos + sf::Vector2f(0.85f * RADIUS, 0.f) - 0.25f * radiusVector);
	m_smallRingRight2.setPosition(pos + sf::Vector2f(0.85f * RADIUS, 0.f) - 0.05f * radiusVector);
	m_smallRingTop1.setPosition(pos - sf::Vector2f(0.f, 0.85f * RADIUS) - 0.25f * radiusVector);
	m_smallRingTop2.setPosition(pos - sf::Vector2f(0.f, 0.85f * RADIUS) - 0.05f * radiusVector);
	m_smallRingBottom1.setPosition(pos + sf::Vector2f(0.f, 0.85f * RADIUS) - 0.25f * radiusVector);
	m_smallRingBottom2.setPosition(pos + sf::Vector2f(0.f, 0.85f * RADIUS) - 0.05f * radiusVector);
}

void SpellSlot::activate()
{
	m_active = true;
	m_inside.setTexture(m_isChopSlot ? 
		g_resourceManager->getTexture(ResourceID::Texture_items) : 
		g_resourceManager->getTexture(ResourceID::Texture_spellicons));
	m_inside.setTextureRect(m_textureRect);
	m_inside.setFillColor(CENDRIC_COLOR_WHITE);
	m_coloredRing.setAngle(360.f);

	m_smallRingLeft1.setFillColor(m_isSelected ? sf::Color::Red : m_color);
	m_smallRingLeft2.setFillColor(m_color);
	m_smallRingRight1.setFillColor(m_isSelected ? sf::Color::Red : m_color);
	m_smallRingRight2.setFillColor(m_color);
	m_smallRingTop1.setFillColor(m_isSelected ? sf::Color::Red : m_color);
	m_smallRingTop2.setFillColor(m_color);
	m_smallRingBottom1.setFillColor(m_isSelected ? sf::Color::Red : m_color);
	m_smallRingBottom2.setFillColor(m_color);
}

void SpellSlot::deactivate()
{
	m_active = false;
	m_inside.setTexture(m_textureInactive);
	m_inside.setTextureRect(sf::IntRect(50, 50, 400, 400));
	m_inside.setFillColor(m_colorBase);
	m_coloredRing.setAngle(0.f);

	m_smallRingLeft1.setFillColor(m_colorBase);
	m_smallRingLeft2.setFillColor(m_colorBase);
	m_smallRingRight1.setFillColor(m_colorBase);
	m_smallRingRight2.setFillColor(m_colorBase);
	m_smallRingTop1.setFillColor(m_colorBase);
	m_smallRingTop2.setFillColor(m_colorBase);
	m_smallRingBottom1.setFillColor(m_colorBase);
	m_smallRingBottom2.setFillColor(m_colorBase);
}

void SpellSlot::render(sf::RenderTarget& renderTarget)
{
	renderTarget.draw(m_outerRing);
	renderTarget.draw(m_coloredRingBase);
	renderTarget.draw(m_coloredRing);
	renderTarget.draw(m_innerRing);
	renderTarget.draw(m_insideBase);
	renderTarget.draw(m_inside);

	renderTarget.draw(m_smallRingLeft1);
	renderTarget.draw(m_smallRingLeft2);
	renderTarget.draw(m_smallRingRight1);
	renderTarget.draw(m_smallRingRight2);
	renderTarget.draw(m_smallRingTop1);
	renderTarget.draw(m_smallRingTop2);
	renderTarget.draw(m_smallRingBottom1);
	renderTarget.draw(m_smallRingBottom2);

	renderTarget.draw(m_inputKey);

	GameObject::renderAfterForeground(renderTarget);
}

void SpellSlot::playAnimation(const sf::Time &cooldown)
{
	if (m_active)
	{
		m_cooldown = cooldown;
		m_animationTime = sf::Time::Zero;
		m_animating = true;
	}
}

void SpellSlot::update(const sf::Time& frameTime)
{
	float flashTime = 0.05f;	// seconds

	if (m_animating)
	{
		float t = m_animationTime.asSeconds() / m_cooldown.asSeconds();
		float angle = lerp(t, 0.f, 360.f);
		// cout << angle << endl;
		m_coloredRing.setAngle(angle);

		m_animationTime += frameTime;

		if (m_animationTime.asSeconds() > m_cooldown.asSeconds())
		{
			if (m_animationTime.asSeconds() < m_cooldown.asSeconds() + flashTime)
			{
				m_coloredRing.setAngle(360.f);
				m_coloredRing.setFillColor(CENDRIC_COLOR_WHITE);
			}
			else
			{
				m_animationTime = sf::Time::Zero;
				m_coloredRing.setAngle(360.f);
				m_coloredRing.setFillColor(m_color);
				m_animating = false;
			}
			
		}
	}
	m_isClicked = false;
	m_isRightClicked = false;
	GameObject::update(frameTime);
}

void SpellSlot::onLeftJustPressed()
{
	if (!m_active) return;
	m_isClicked = true;
	g_inputController->lockAction();
}

void SpellSlot::onRightClick()
{
	if (!m_active) return;
	m_isRightClicked = true;
	g_inputController->lockAction();
}

GameObjectType SpellSlot::getConfiguredType() const
{
	return GameObjectType::_Interface;
}

SpellType SpellSlot::getSpellType() const
{
	return m_spellType;
}

SpellID SpellSlot::getSpellID() const
{
	return m_spellID;
}

const sf::IntRect& SpellSlot::getTextureRect() const
{
	return m_textureRect;
}

int SpellSlot::getNr() const
{
	return m_nr;
}

void SpellSlot::setNr(int nr)
{
	m_nr = nr;
}