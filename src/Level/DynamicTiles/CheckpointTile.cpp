#include "Level/DynamicTiles/CheckpointTile.h"
#include "Level/LevelMainCharacter.h"
#include "Spells/Spell.h"
#include "Screens/LevelScreen.h"
#include "GameObjectComponents/InteractComponent.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Checkpoint, CheckpointTile)

const float CheckpointTile::ACTIVATE_RANGE = 100.f;

CheckpointTile::CheckpointTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {
	m_interactComponent = new InteractComponent(g_textProvider->getText("Checkpoint"), this, m_mainChar);
	m_interactComponent->setInteractRange(ACTIVATE_RANGE);
	m_interactComponent->setInteractText("ToActivate");
	m_interactComponent->setOnInteract(std::bind(&CheckpointTile::onLeftClick, this));
	m_interactComponent->setTooltipHeight(30.f);
	addComponent(m_interactComponent);
}

bool CheckpointTile::init(const LevelTileProperties& properties) {
	setSpriteOffset(sf::Vector2f(-15.f, -30.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));

	m_isMimic = contains(properties, std::string("mimic"));

	return true;
}

void CheckpointTile::loadAnimation(int skinNr) {
	m_isCollidable = false;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, skinNr * 80, 80, 80));
	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* activatedAnimation = new Animation(sf::seconds(0.2f));
	activatedAnimation->setSpriteSheet(tex);
	for (int i = 1; i < 5; i++) {
		activatedAnimation->addFrame(sf::IntRect(i * 80, skinNr * 80, 80, 80));
	}
	for (int i = 3; i > 0; i--) {
		activatedAnimation->addFrame(sf::IntRect(i * 80, skinNr * 80, 80, 80));
	}
	addAnimation(GameObjectState::Active, activatedAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);

	if (fastIntersect(*m_mainChar->getBoundingBox(), *getBoundingBox())) {
		setActive(true, false);
	}
}

void CheckpointTile::activateMimic() {
	Enemy* gargoyle = dynamic_cast<LevelScreen*>(m_screen)->spawnEnemy(EnemyID::Gargoyle, getPosition() - sf::Vector2f(0.f, 100.f));
	std::map<std::string, int> items;
	items.insert({ "mi_gargoyle_dust", 1 });
	gargoyle->setLoot(items, 2);
	g_resourceManager->playSound(getSoundPath());
	setDisposed();
}

void CheckpointTile::onHit(Spell* spell) {
	if (!m_isMimic || (spell->getSpellID() != SpellID::Telekinesis && spell->getSpellID() != SpellID::Chop)) return;

	activateMimic();
}

void CheckpointTile::onRightClick() {
	onLeftClick();
}

void CheckpointTile::onLeftClick() {
	if (m_state == GameObjectState::Active) return;
	
	g_inputController->lockAction();
	if (dist(m_mainChar->getCenter(), getCenter()) > ACTIVATE_RANGE) {
		m_screen->setNegativeTooltip("OutOfRange");
		return;
	}

	if (m_isMimic) {
		// nobody expects the spanish inquisition
		activateMimic();
		return;
	}

	WorldCollisionQueryRecord rec;
	rec.boundingBox = *(m_mainChar->getBoundingBox());
	rec.boundingBox.left = getBoundingBox()->left + getBoundingBox()->width / 2.f - rec.boundingBox.width / 2.f;
	rec.boundingBox.top = getBoundingBox()->top + (getBoundingBox()->height - rec.boundingBox.height);
	if (!m_level->collides(rec)) {
		if (LevelScreen* screen = dynamic_cast<LevelScreen*>(getScreen())) {
			screen->getCharacterCore()->setLevel(sf::Vector2f(rec.boundingBox.left, rec.boundingBox.top), m_level->getID());
			screen->getCharacterCore()->autosave();
			screen->setTooltipText("CheckpointReached", COLOR_GOOD, true);
			screen->clearConsumedFood();
		}
	}
	else {
		g_logger->logError("CheckpointTile", "Setting this checkpoint would stuck our mob, it is not saved.");
		return;
	}

	// deactivate other tiles
	for (auto& it : *m_level->getDynamicTiles()) {
		CheckpointTile* tile = dynamic_cast<CheckpointTile*>(it);
		if (tile == nullptr || tile == this) continue;
		tile->setActive(false);
	}

	// activate this 
	setActive(true);
}

void CheckpointTile::setActive(bool active, bool playSound) {
	setState(active ? GameObjectState::Active : GameObjectState::Idle);
	if (active && playSound) {
		g_resourceManager->playSound(getSoundPath());
	} 
	m_interactComponent->setInteractable(!active);
	if (active) {
		dynamic_cast<LevelScreen*>(m_screen)->notifyLeveloverlayReload();
	}
}

std::string CheckpointTile::getSpritePath() const {
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_checkpoint.png";
}

std::string CheckpointTile::getSoundPath() const {
	return "res/sound/tile/gargoyle.ogg";
}