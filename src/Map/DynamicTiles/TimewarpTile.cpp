#include "Map/DynamicTiles/TimewarpTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "Registrar.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GUI/YesOrNoForm.h"

REGISTER_MAP_DYNAMIC_TILE(MapDynamicTileID::Timewarp, TimewarpTile)

const float TimewarpTile::ACTIVATE_RANGE = 100.f;

TimewarpTile::TimewarpTile(MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	auto interactComponent = new InteractComponent(g_textProvider->getText("TimeWarp"), this, m_mainChar);
	interactComponent->setInteractRange(ACTIVATE_RANGE);
	interactComponent->setInteractText("ToInteract");
	interactComponent->setOnInteract(std::bind(&TimewarpTile::onInteract, this));
	addComponent(interactComponent);
}

bool TimewarpTile::init(const MapTileProperties& properties) {
	setBoundingBox(sf::FloatRect(0.f, 0.f,
		TILE_SIZE_F,
		TILE_SIZE_F));
	
	return true;
}

void TimewarpTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

void TimewarpTile::onInteract() {
	float width = 450;
	float height = 200;
	m_yesOrNoForm = new YesOrNoForm(sf::FloatRect(0.5f * (WINDOW_WIDTH - width), 0.5f * (WINDOW_HEIGHT - height), width, height));
	m_yesOrNoForm->setMessage("TimeWarpText");
	m_yesOrNoForm->setOnNoClicked(std::bind(&TimewarpTile::onNo, this));
	m_yesOrNoForm->setOnYesClicked(std::bind(&TimewarpTile::activateTimewarp, this));
	m_screen->addObject(m_yesOrNoForm);
	m_screen->setAllButtonsEnabled(false);
}

void TimewarpTile::onNo() {
	m_screen->setAllButtonsEnabled(true);
	m_yesOrNoForm->setDisposed();
}

void TimewarpTile::activateTimewarp() {
	// a very custom set-back-to-throne-room thing.
	auto cc = m_screen->getCharacterCore();
	cc->timewarpToThrone();

	dynamic_cast<WorldScreen*>(m_screen)->exitWorld();
	cc->setMap(sf::Vector2f(640.f, 400.f), "res/map/gandriacastle/gandriacastle.tmx");
	m_screen->setNextScreen(new LoadingScreen(cc));
	
	g_resourceManager->playSound(getSoundPath());
}

void TimewarpTile::onLeftClick() {
	onRightClick();
}

void TimewarpTile::onRightClick() {
	bool inRange = dist(m_mainChar->getCenter(), getCenter()) <= ACTIVATE_RANGE;

	if (inRange) {
		onInteract();
	}
	else {
		m_screen->setNegativeTooltip("OutOfRange");
	}
	g_inputController->lockAction();
}

std::string TimewarpTile::getSpritePath() const {
	return "res/texture/map_dynamic_tiles/spritesheet_tiles_timewarp.png";
}

std::string TimewarpTile::getSoundPath() const {
	return GlobalResource::SOUND_TELEPORT;
}

