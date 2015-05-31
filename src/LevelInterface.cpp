#include "LevelInterface.h"

LevelInterface::LevelInterface(CharacterCore* core)
{
	m_core = core;
	m_attributes = &(core->getData().attributes);
	initInterfaceBars();
	initCharacterInfo();
	initInventory();
}

void LevelInterface::render(sf::RenderTarget& target)
{
	sf::View oldView = target.getView();
	target.setView(target.getDefaultView());

	renderInterfaceBars(target);
	renderCharacterInfo(target);
	renderInventory(target);

	target.setView(oldView);
}

void LevelInterface::renderInterfaceBars(sf::RenderTarget& target) const
{
	target.draw(m_hpBarOutline);
	target.draw(m_hpBar);
	target.draw(m_manaBarOutline);
	target.draw(m_manaBar);
}

void LevelInterface::renderCharacterInfo(sf::RenderTarget& target) const
{
	if (m_showCharacterInfo)
	{
		target.draw(m_characterInfoShape);
		target.draw(m_characterInfoText);
	}
}

void LevelInterface::renderInventory(sf::RenderTarget& target) const
{
	if (m_showInventory)
	{
		target.draw(m_inventoryShape);
		target.draw(m_inventoryText);
	}
}

void LevelInterface::update(const sf::Time& frameTime)
{
	updateInterfaceBars();
	updateInventory();
	updateCharacterInfo();
}

void LevelInterface::updateInterfaceBars()
{
	m_hpBar.setSize(sf::Vector2f(BAR_WIDTH * (static_cast<float>(m_attributes->currentHealthPoints) / m_attributes->maxHealthPoints), BAR_HEIGHT));
	m_manaBar.setSize(sf::Vector2f(BAR_WIDTH * (static_cast<float>(m_attributes->currentManaPoints) / m_attributes->maxManaPoints), BAR_HEIGHT));
}

void LevelInterface::updateCharacterInfo()
{
	if (g_inputController->isKeyJustPressed(Key::CharacterInfo))
	{
		m_showCharacterInfo = !m_showCharacterInfo;
	}
	if (m_showCharacterInfo)
	{
		// update text
		std::string infoText = "";
		infoText.append(g_textProvider->getText(Texts::CharacterInfo));
		infoText.append("\n\n");
		// health
		infoText.append(g_textProvider->getText(Texts::Attribute_health));
		infoText.append(": ");
		infoText.append(std::to_string(m_attributes->currentHealthPoints));
		infoText.append("/");
		infoText.append(std::to_string(m_attributes->maxHealthPoints));
		infoText.append("\n");
		// mana
		infoText.append(g_textProvider->getText(Texts::Attribute_mana));
		infoText.append(": ");
		infoText.append(std::to_string(m_attributes->currentManaPoints));
		infoText.append("/");
		infoText.append(std::to_string(m_attributes->maxManaPoints));
		infoText.append("\n");
		// dmg 
		infoText.append(g_textProvider->getText(Texts::Attribute_damagePhysical));
		infoText.append(": ");
		infoText.append(std::to_string(m_attributes->damagePhysical));
		infoText.append("\n");
		infoText.append(g_textProvider->getText(Texts::Attribute_damageFire));
		infoText.append(": ");
		infoText.append(std::to_string(m_attributes->damageFire));
		infoText.append("\n");
		infoText.append(g_textProvider->getText(Texts::Attribute_damageIce));
		infoText.append(": ");
		infoText.append(std::to_string(m_attributes->damageIce));
		infoText.append("\n");
		// resistance
		infoText.append(g_textProvider->getText(Texts::Attribute_resistancePhysical));
		infoText.append(": ");
		infoText.append(std::to_string(m_attributes->resistancePhysical));
		infoText.append("\n");
		infoText.append(g_textProvider->getText(Texts::Attribute_resistanceFire));
		infoText.append(": ");
		infoText.append(std::to_string(m_attributes->resistanceFire));
		infoText.append("\n");
		infoText.append(g_textProvider->getText(Texts::Attribute_resistanceIce));
		infoText.append(": ");
		infoText.append(std::to_string(m_attributes->resistanceIce));
		infoText.append("\n");
		m_characterInfoText.setString(infoText);
	}
}

void LevelInterface::updateInventory()
{
	if (g_inputController->isKeyJustPressed(Key::Inventory))
	{
		m_showInventory = !m_showInventory;
	}
	if (m_showInventory)
	{
		// update text
		std::string inventoryText = "";
		inventoryText.append(g_textProvider->getText(Texts::Inventory));
		inventoryText.append("\n\n");
		// reload
		m_core->loadItems();
		for (auto it : m_core->getData().items)
		{
			inventoryText.append(g_textProvider->getText(m_core->getItem(it.first).getName()));
			inventoryText.append(": ");
			inventoryText.append(std::to_string(it.second));
			inventoryText.append("\n");
		}
		m_inventoryText.setString(inventoryText);
	}
}

void LevelInterface::setSpellManager(SpellManager* spellManager)
{
	m_spellManager = spellManager;
}

void LevelInterface::initInterfaceBars()
{
	m_hpBar.setFillColor(sf::Color::Red);
	m_hpBar.setSize(sf::Vector2f(0, BAR_HEIGHT));
	m_hpBar.setPosition(sf::Vector2f(BAR_LEFT + BAR_OUTLINE_THICKNESS, BAR_TOP + BAR_OUTLINE_THICKNESS));

	m_hpBarOutline.setOutlineColor(sf::Color(150, 0, 0, 255));
	m_hpBarOutline.setOutlineThickness(BAR_OUTLINE_THICKNESS);
	m_hpBarOutline.setFillColor(sf::Color::Transparent);
	m_hpBarOutline.setPosition(sf::Vector2f(BAR_LEFT + BAR_OUTLINE_THICKNESS, BAR_TOP + BAR_OUTLINE_THICKNESS));
	m_hpBarOutline.setSize(sf::Vector2f(BAR_WIDTH, BAR_HEIGHT));

	m_manaBar.setFillColor(sf::Color::Blue);
	m_manaBar.setSize(sf::Vector2f(0, BAR_HEIGHT));
	m_manaBar.setPosition(sf::Vector2f(BAR_LEFT + BAR_OUTLINE_THICKNESS, BAR_TOP + 3 * BAR_OUTLINE_THICKNESS + BAR_HEIGHT));
	
	m_manaBarOutline.setOutlineColor(sf::Color(0, 0, 150, 255));
	m_manaBarOutline.setOutlineThickness(BAR_OUTLINE_THICKNESS);
	m_manaBarOutline.setFillColor(sf::Color::Transparent);
	m_manaBarOutline.setPosition(sf::Vector2f(BAR_LEFT + BAR_OUTLINE_THICKNESS, BAR_TOP + 3 * BAR_OUTLINE_THICKNESS + BAR_HEIGHT));
	m_manaBarOutline.setSize(sf::Vector2f(BAR_WIDTH, BAR_HEIGHT));
}

void LevelInterface::initCharacterInfo()
{
	m_characterInfoShape.setFillColor(sf::Color(50, 50, 50, 100));
	m_characterInfoShape.setSize(sf::Vector2f(INFO_WIDTH, INFO_HEIGHT));
	m_characterInfoShape.setOutlineThickness(INFO_OUTLINE_THICKNESS);
	m_characterInfoShape.setOutlineColor(CENDRIC_COLOR_PURPLE);
	m_characterInfoShape.setPosition(sf::Vector2f(INFO_LEFT, INFO_TOP));

	m_characterInfoText = sf::Text(
		"",
		(*g_resourceManager->getFont(ResourceID::Font_copperplateGothicBold)));
	m_characterInfoText.setPosition(sf::Vector2f(INFO_LEFT, INFO_TOP));
	m_characterInfoText.setColor(sf::Color::White);
	m_characterInfoText.setCharacterSize(20);
}

void LevelInterface::initInventory()
{
	m_inventoryShape.setFillColor(sf::Color(50, 50, 50, 100));
	m_inventoryShape.setSize(sf::Vector2f(INFO_WIDTH, INFO_HEIGHT));
	m_inventoryShape.setOutlineThickness(INFO_OUTLINE_THICKNESS);
	m_inventoryShape.setOutlineColor(CENDRIC_COLOR_PURPLE);
	m_inventoryShape.setPosition(sf::Vector2f(INFO_LEFT + INFO_WIDTH + INVENTORY_LEFT, INVENTORY_TOP));

	m_inventoryText = sf::Text(
		"",
		(*g_resourceManager->getFont(ResourceID::Font_copperplateGothicBold)));
	m_inventoryText.setPosition(sf::Vector2f(INFO_LEFT + INFO_WIDTH + INVENTORY_LEFT, INVENTORY_TOP));
	m_inventoryText.setColor(sf::Color::White);
	m_inventoryText.setCharacterSize(20);
}

