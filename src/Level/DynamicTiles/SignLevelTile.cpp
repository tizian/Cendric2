#include "Level/DynamicTiles/SignLevelTile.h"
#include "Registrar.h"
#include "Screens/WorldScreen.h"
#include "GameObjectComponents/InteractComponent.h"

const float SignLevelTile::TOOLTIP_TOP = 20.f;
const float SignLevelTile::READ_RANGE = 50.f;
const sf::Time SignLevelTile::TOOLTIP_WINDOW_TIME = sf::seconds(3.f);

SignLevelTile::SignLevelTile(const SignData& data, LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {
	m_data = data;
	m_tooltipWindow.setText(g_textProvider->getText(data.text, "sign"));
	m_tooltipWindow.setTextOffset(sf::Vector2f(30.f, 10.f));
	m_tooltipWindow.setTextAlignment(TextAlignment::Center);
	m_showTooltip = false;

	m_interactComponent = new InteractComponent(g_textProvider->getText("Sign"), this, m_mainChar);
	m_interactComponent->setInteractRange(READ_RANGE);
	m_interactComponent->setInteractText("ToRead");
	m_interactComponent->setOnInteract(std::bind(&SignLevelTile::onInteract, this));
	addComponent(m_interactComponent);
}

void SignLevelTile::onInteract() {
	m_tooltipWindowTime = TOOLTIP_WINDOW_TIME;
	m_tooltipWindow.setPosition(sf::Vector2f(
		getPosition().x, 
		getPosition().y - m_tooltipWindow.getSize().y - TOOLTIP_TOP));
}

void SignLevelTile::update(const sf::Time& frameTime) {
	updateTime(m_tooltipWindowTime, frameTime);
	m_showTooltip = m_tooltipWindowTime > sf::Time::Zero;
	m_interactComponent->setInteractable(!m_showTooltip);

	LevelDynamicTile::update(frameTime);
}

void SignLevelTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, 30.f));
	setPositionOffset(sf::Vector2f(0.f, 10.f));
	setSpriteOffset(sf::Vector2f(0.f, -10.f));
}

void SignLevelTile::loadAnimation(int skinNr) {
	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_sign_level));
	idleAnimation->addFrame(sf::IntRect(0, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);
}

void SignLevelTile::setPosition(const sf::Vector2f& pos) {
	LevelDynamicTile::setPosition(pos);
	m_tooltipWindow.setPosition(sf::Vector2f(pos.x, pos.y - m_tooltipWindow.getSize().y - TOOLTIP_TOP));
}

void SignLevelTile::onMouseOver() {
	setSpriteColor(COLOR_INTERACTIVE, sf::milliseconds(100));
	m_showTooltip = true;
	m_interactComponent->setInteractable(false);
	// update tooltip position
	sf::Vector2f mouse = g_inputController->getMousePosition();
	sf::Vector2f size = m_tooltipWindow.getSize();
	m_tooltipWindow.setPosition(sf::Vector2f(mouse.x - 0.5f * size.x, mouse.y - size.y - TOOLTIP_TOP));
}

void SignLevelTile::renderAfterForeground(sf::RenderTarget& renderTarget) {
	LevelDynamicTile::renderAfterForeground(renderTarget);
	if (m_showTooltip) {
		m_tooltipWindow.render(renderTarget);
	}
}