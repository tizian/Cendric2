#include "GUI/ModifierSlotClone.h" 

using namespace std;

ModifierSlotClone::ModifierSlotClone(const ModifierSlot* original)
{
	m_original = original->getModifier();

	setBoundingBox(sf::FloatRect(0.f, 0.f, ModifierSlot::SIDE_LENGTH, ModifierSlot::SIDE_LENGTH));
	setDebugBoundingBox(sf::Color::Red);
	setInputInDefaultView(true);

	m_inside.setSize(sf::Vector2f(ModifierSlot::SIDE_LENGTH, ModifierSlot::SIDE_LENGTH));
	m_inside.setTexture(g_resourceManager->getTexture(ResourceID::Texture_gems));
	m_inside.setTextureRect(sf::IntRect((m_original.level - 1) * 50, static_cast<int>(m_original.type) * 50, 50, 50));

	m_outside.setSize(sf::Vector2f(ModifierSlot::SIDE_LENGTH, ModifierSlot::SIDE_LENGTH));
	m_outside.setFillColor(CENDRIC_COLOR_TRANS_GREY);
	m_outside.setOutlineThickness(ModifierSlot::MARGIN);
	m_outside.setOutlineColor(sf::Color::Red);
}

void ModifierSlotClone::setPosition(const sf::Vector2f& pos)
{
	GameObject::setPosition(pos);
	m_inside.setPosition(pos);
	m_outside.setPosition(pos);
}

void ModifierSlotClone::render(sf::RenderTarget& renderTarget)
{
	renderTarget.draw(m_outside);
	renderTarget.draw(m_inside);
}

GameObjectType ModifierSlotClone::getConfiguredType() const
{
	return GameObjectType::_Interface;
}

const SpellModifier&  ModifierSlotClone::getModifier() const
{
	return m_original;;
}