#include "Level/DynamicTiles/SignLevelTile.h"
#include "Registrar.h"
#include "Screens/WorldScreen.h"
#include "GameObjectComponents/InteractComponent.h"
#include "Screens/LevelScreen.h"
#include "GUI/Hints.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Sign, SignLevelTile)

const float SignLevelTile::TOOLTIP_TOP = 20.f;
const float SignLevelTile::READ_RANGE = 50.f;
const sf::Time SignLevelTile::TOOLTIP_WINDOW_TIME = sf::seconds(3.f);

SignLevelTile::SignLevelTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {	
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

bool SignLevelTile::init(const LevelTileProperties& properties) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, 30.f));
	setPositionOffset(sf::Vector2f(0.f, 10.f));
	setSpriteOffset(sf::Vector2f(0.f, -10.f));

	if (contains(properties, std::string("hint"))) {
		std::string text = properties.at("hint");
		m_tooltipWindow.setText(getHintDescription(text));
		m_screen->getCharacterCore()->learnHint(text);
	}
	else if (contains(properties, std::string("text"))) {
		std::string text = properties.at("text");
		m_tooltipWindow.setText(g_textProvider->getText(text, "sign"));
	}
	else {
		return false;
	}

	return true;
}

void SignLevelTile::loadAnimation(int skinNr) {
	Animation* idleAnimation = new Animation();
	std::string path = getSpritePath();
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

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

std::string SignLevelTile::getSpritePath() const {
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_sign.png";
}