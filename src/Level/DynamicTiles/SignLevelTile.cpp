#include "Level/DynamicTiles/SignLevelTile.h"
#include "Registrar.h"
#include "Screens/WorldScreen.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GameObjectComponents/TooltipWindowComponent.h"
#include "Screens/LevelScreen.h"
#include "GUI/Hints.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Sign, SignLevelTile)

const float SignLevelTile::TOOLTIP_TOP = 20.f;
const float SignLevelTile::READ_RANGE = 50.f;

SignLevelTile::SignLevelTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {	
	m_interactComponent = new InteractComponent(g_textProvider->getText("Sign"), this, m_mainChar);
	m_interactComponent->setInteractRange(READ_RANGE);
	m_interactComponent->setInteractText("ToRead");
	m_interactComponent->setOnInteract(std::bind(&SignLevelTile::onInteract, this));
	addComponent(m_interactComponent);

	m_tooltipComponent = new TooltipWindowComponent("", this);
	m_tooltipComponent->setTextAlignment(TextAlignment::Center);
	m_tooltipComponent->setTextOffset(sf::Vector2f(30.f, 10.f));
	addComponent(m_tooltipComponent);
}

void SignLevelTile::onInteract() {
	m_tooltipComponent->setCurrentTooltipTime(sf::seconds(3.f));
}

void SignLevelTile::update(const sf::Time& frameTime) {
	m_interactComponent->setInteractable(!m_tooltipComponent->isShowingTooltip());

	LevelDynamicTile::update(frameTime);
}

bool SignLevelTile::init(const LevelTileProperties& properties) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, 30.f));
	setPositionOffset(sf::Vector2f(0.f, 10.f));
	setSpriteOffset(sf::Vector2f(0.f, -10.f));

	if (contains(properties, std::string("hint"))) {
		std::string text = properties.at("hint");
		m_tooltipComponent->setTooltipText(getHintDescription(text));
		m_screen->getCharacterCore()->learnHint(text);
	}
	else if (contains(properties, std::string("text"))) {
		std::string text = properties.at("text");
		m_tooltipComponent->setTooltipText(g_textProvider->getText(text, "sign"));
	}
	else {
		return false;
	}

	if (contains(properties, std::string("name"))) {
		m_interactComponent->setTooltipText(g_textProvider->getText(properties.at("name"), "sign_name"));
	}

	m_tooltipComponent->setWindowOffset(sf::Vector2f(0.f, -m_tooltipComponent->getHeight() - TOOLTIP_TOP));

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

void SignLevelTile::onMouseOver() {
	setSpriteColor(COLOR_INTERACTIVE, sf::milliseconds(100));
	m_interactComponent->setInteractable(false);
	GameObject::onMouseOver();
}

std::string SignLevelTile::getSpritePath() const {
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_sign.png";
}