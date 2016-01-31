#include "Level/DynamicTiles/ModifierTile.h"
#include "Level/LevelMainCharacter.h"
#include "Spell.h"
#include "Screens/LevelScreen.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Modifier, ModifierTile)

ModifierTile::ModifierTile(Level* level) : LevelDynamicTile(level) {
	m_lightObject = new LightObject(LightData(sf::Vector2f(), sf::Vector2f(200.f, 200.f), 0.5f));
}

void ModifierTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 50.f, 50.f));

	loadParticleSystem();
}

void ModifierTile::loadAnimation(int skinNr) {
	m_isCollidable = false;
	
	sf::IntRect rect = sf::IntRect((m_modifier.level - 1) * 50, 50, 50, 50);
	m_animatedSprite.setColor(SpellModifier::getSpellModifierColor(m_modifier.type));

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_gems));
	idleAnimation.addFrame(rect);
	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation activatedAnimation(sf::seconds(10.f));
	activatedAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_gems));
	activatedAnimation.addFrame(sf::IntRect()); // idle
	addAnimation(GameObjectState::Active, activatedAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);
}

void ModifierTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);

	if (m_state == GameObjectState::Active) {
		GameObject::updateTime(m_particleTime, frameTime);
		if (m_particleTime == sf::Time::Zero) {
			setDisposed();
		}
	}

	m_ps->update(frameTime);
}

void ModifierTile::setScreen(Screen* screen) {
	LevelDynamicTile::setScreen(screen);
	screen->addObject(m_lightObject);
}

void ModifierTile::setPosition(const sf::Vector2f& pos) {
	LevelDynamicTile::setPosition(pos);
	if (m_lightObject != nullptr) m_lightObject->setPosition(pos + sf::Vector2f(getBoundingBox()->width / 2.f, getBoundingBox()->height / 2.f));
	updateParticleSystemPosition();
}

void ModifierTile::render(sf::RenderTarget& target) {
	m_ps->render(target);
	LevelDynamicTile::render(target);
}

void ModifierTile::setDisposed() {
	LevelDynamicTile::setDisposed();
	if (m_lightObject != nullptr) m_lightObject->setDisposed();
}

void ModifierTile::setModifier(const SpellModifier& modififer) {
	m_modifier = modififer;
}

void ModifierTile::updateParticleSystemPosition() {
	if (m_pointGenerator == nullptr) return;
	m_pointGenerator->center.x = getPosition().x + getBoundingBox()->width / 2;
	m_pointGenerator->center.y = getPosition().y + getBoundingBox()->height / 2;
}

void ModifierTile::addModifier() {
	m_state = GameObjectState::Active;
	setCurrentAnimation(getAnimation(m_state), false);

	LevelScreen* screen = dynamic_cast<LevelScreen*>(getScreen());

	screen->getCharacterCore()->learnModifier(m_modifier);
	std::string text = g_textProvider->getText("ModifierLearned");
	text.append(": ");
	text.append(g_textProvider->getText(EnumNames::getSpellModifierTypeName(m_modifier.type)));
	text.append(", ");
	text.append(g_textProvider->getText("Level"));
	text.append(" ");
	text.append(std::to_string(m_modifier.level));
	screen->setTooltipText(text, sf::Color::Green, true);

	m_ps->emitRate = 0;

	m_lightObject->setDisposed();
	m_lightObject = nullptr;
}
 
void ModifierTile::onHit(LevelMovableGameObject* mob) {
	if (m_state == GameObjectState::Active) return;
	if (LevelMainCharacter* character = dynamic_cast<LevelMainCharacter*>(mob)) {
		addModifier();
	}
}

void ModifierTile::onHit(Spell* spell) {
	if (m_state == GameObjectState::Active) return;
	switch (spell->getSpellID()) {
	case SpellID::Telekinesis:
		addModifier();
		spell->setDisposed();
		break;
	default:
		break;
	}
}

void ModifierTile::loadParticleSystem() {
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(300, g_resourceManager->getTexture(ResourceID::Texture_Particle_star)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 100.f * m_modifier.level / 3.0f;

	// Generators
	auto posGen = m_ps->addGenerator<particles::PointPositionGenerator>();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	m_pointGenerator = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 10.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 1.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = SpellModifier::getSpellModifierColor(m_modifier.type);
	colGen->maxStartCol = sf::Color(255, 255, 255, 255);
	colGen->minEndCol = sf::Color(0, 0, 0, 0);
	colGen->maxEndCol = SpellModifier::getSpellModifierColor(m_modifier.type);
	colGen->maxEndCol.a = 0;

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartVel = 30.f * m_modifier.level / 3;
	velGen->maxStartVel = 60.f * m_modifier.level / 3;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 3.f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}