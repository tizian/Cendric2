#include "Level/LevelMovableGameObject.h"
#include "Spells/SpellManager.h"
#include "Level/Level.h"
#include "Level/MOBBehavior/MovingBehavior.h"
#include "Level/MOBBehavior/AttackingBehavior.h"
#include "Level/DamageNumbers.h"

LevelMovableGameObject::LevelMovableGameObject(const Level* level) : MovableGameObject() {
	m_level = level;
	m_foodAttributes.first = sf::Time::Zero;
	m_foodAttributes.second = ZERO_ATTRIBUTES;
}

LevelMovableGameObject::~LevelMovableGameObject() {
	delete m_spellManager;
	delete m_movingBehavior;
	delete m_attackingBehavior;
	if (m_damageNumbers) {
		delete m_damageNumbers;
	}
}

void LevelMovableGameObject::update(const sf::Time& frameTime) {
	// update times
	updateTime(m_stunnedTime, frameTime);
	updateTime(m_fearedTime, frameTime);

	m_movingBehavior->update(frameTime);
	m_attackingBehavior->update(frameTime);

	m_level->collideWithDynamicTiles(this, *getBoundingBox());
	m_spellManager->update(frameTime);

	m_displayDamageNumbers = g_resourceManager->getConfiguration().isDisplayDamageNumbers;
	if (m_displayDamageNumbers && m_damageNumbers) {
		m_damageNumbers->update(frameTime);
	}

	MovableGameObject::update(frameTime);

	if (!m_isDead) {
		updateAttributes(frameTime);
	}
	setAccelerationX(0.f);
}

void LevelMovableGameObject::renderAfterForeground(sf::RenderTarget& target) {
	MovableGameObject::renderAfterForeground(target);
	if (m_displayDamageNumbers && m_damageNumbers) {
		m_damageNumbers->render(target);
	}
}

void LevelMovableGameObject::updateAttributes(const sf::Time& frameTime) {
	// update food attributes
	if (m_foodAttributes.first > sf::Time::Zero) {
		m_foodAttributes.first -= frameTime;
		if (m_foodAttributes.first <= sf::Time::Zero) {
			m_foodAttributes.first = sf::Time::Zero;
			m_attributes.removeBean(m_foodAttributes.second);
		}
	}

	// update buff attributes
	for (size_t i = 0; i < m_buffAttributes.size();/* don't increment here, we remove on the fly */) {
		m_buffAttributes[i].first -= frameTime;
		if (m_buffAttributes[i].first <= sf::Time::Zero) {
			m_attributes.removeBean(m_buffAttributes[i].second);
			m_buffAttributes.erase(m_buffAttributes.begin() + i);
		}
		else {
			i++;
		}
	}

	// health regeneration
	m_timeSinceRegeneration += frameTime;
	if (m_timeSinceRegeneration >= sf::seconds(1)) {
		m_timeSinceRegeneration -= sf::seconds(1);
		m_attributes.currentHealthPoints += m_attributes.healthRegenerationPerS;
		
		if (m_damageNumbers) {
			const sf::Vector2f& pos = getPosition();
			const sf::Vector2f& size = getSize();
			sf::Vector2f numberPos = sf::Vector2f(pos.x + 0.5f * size.x, pos.y);

			if (m_attributes.healthRegenerationPerS > 0 && m_damageNumbers) {
				m_damageNumbers->emitNumber(m_attributes.healthRegenerationPerS, numberPos, DamageNumberType::HealOverTime);
			}
			else if (m_attributes.healthRegenerationPerS < 0) {
				m_damageNumbers->emitNumber(std::abs(m_attributes.healthRegenerationPerS), numberPos, DamageNumberType::DamageOverTime);
			}
		}
		

		if (m_attributes.currentHealthPoints > m_attributes.maxHealthPoints) {
			m_attributes.currentHealthPoints = m_attributes.maxHealthPoints;
		}
	}

	// update debuff attributes
	for (size_t i = 0; i < m_dots.size();/* don't increment here, we remove on the fly */) {
		int prevSecond = static_cast<int>(std::floor(m_dots[i].duration.asSeconds()));
		m_dots[i].duration -= frameTime;
		int thisSecond = std::max(-1, static_cast<int>(std::floor(m_dots[i].duration.asSeconds())));
		addDamage(m_dots[i].damage * (prevSecond - thisSecond), m_dots[i].damageType, true);
		if (m_dots[i].duration <= sf::Time::Zero) {
			m_dots.erase(m_dots.begin() + i);
		}
		else {
			i++;
		}
	}
}

sf::Vector2f LevelMovableGameObject::getConfiguredSpellOffset() const {
	return sf::Vector2f(0.f, 10.f);
}

void LevelMovableGameObject::addAttributes(const sf::Time& duration, const AttributeData& attributes) {
	m_attributes.addBean(attributes);
	m_buffAttributes.push_back(std::pair<sf::Time, AttributeData>(duration, attributes));
}

void LevelMovableGameObject::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	m_movingBehavior->calculateUnboundedVelocity(frameTime, nextVel);
}

void LevelMovableGameObject::addDamage(int damage_, DamageType damageType, bool overTime) {
	int damage = 0;
	switch (damageType) {
	case DamageType::Physical:
		damage = static_cast<int>(damage_ * m_attributes.physicalMultiplier);
		break;
	case DamageType::Ice:
		damage = static_cast<int>(damage_ * m_attributes.iceMultiplier);
		break;
	case DamageType::Fire:
		damage = static_cast<int>(damage_ * m_attributes.fireMultiplier);
		break;
	case DamageType::Shadow:
		damage = static_cast<int>(damage_ * m_attributes.shadowMultiplier);
		break;
	case DamageType::Light:
		damage = static_cast<int>(damage_ * m_attributes.lightMultiplier);
		break;
	default:
		break;
	}

	if (m_isDead || damage <= 0) return;

	if (m_damageNumbers) {
		const sf::Vector2f& pos = getPosition();
		const sf::Vector2f& size = getSize();
		m_damageNumbers->emitNumber(damage, sf::Vector2f(pos.x + 0.5f * size.x, pos.y), overTime ? DamageNumberType::DamageOverTime : DamageNumberType::Damage);
	}
	
	m_attributes.currentHealthPoints = std::max(0, std::min(m_attributes.maxHealthPoints, m_attributes.currentHealthPoints - damage));
	if (m_attributes.currentHealthPoints == 0) {
		setDead();
	}
	setSpriteColor(COLOR_DAMAGED, sf::milliseconds(400));
}

void LevelMovableGameObject::addDamageOverTime(DamageOverTimeData& data) {
	if (m_isDead || data.damageType == DamageType::VOID) return;
	// set the time for the first tick down, the first tick should not
	// hurt the mob. 
	data.duration -= sf::milliseconds(1);
	m_dots.push_back(data);
}

void LevelMovableGameObject::addHeal(int heal, bool overTime) {
	if (m_isDead || heal <= 0) return;

	if (m_damageNumbers) {
		const sf::Vector2f& pos = getPosition();
		const sf::Vector2f& size = getSize();
		m_damageNumbers->emitNumber(heal, sf::Vector2f(pos.x + 0.5f * size.x, pos.y), overTime ? DamageNumberType::HealOverTime : DamageNumberType::Heal);
	}

	m_attributes.currentHealthPoints = std::max(0, std::min(m_attributes.maxHealthPoints, m_attributes.currentHealthPoints + heal));
	setSpriteColor(COLOR_HEALED, sf::milliseconds(200));
}

void LevelMovableGameObject::setFeared(const sf::Time& fearedTime) {
	if (m_isDead) return;
	m_fearedTime = fearedTime;
}

void LevelMovableGameObject::setStunned(const sf::Time& stunnedTime) {
	if (m_isDead) return;
	m_stunnedTime = stunnedTime;
}

void LevelMovableGameObject::onHit(Spell* spell) {
	if (m_isDead) {
		return;
	}

	spell->execOnHit(this);
	if (spell->getDamageType() == DamageType::VOID) return;
	addDamage(spell->getDamage(), spell->getDamageType(), false);
	if (spell->getDamagePerSecond() > 0.f && spell->getDuration() > sf::Time::Zero) {
		DamageOverTimeData data;
		data.damage = spell->getDamagePerSecond();
		data.damageType = spell->getDamageType();
		data.duration = spell->getDuration();
		addDamageOverTime(data);
	}
}

void LevelMovableGameObject::setDead() { 
	if (m_isImmortal) return;
	// dispose the spells that this mob is an owner of and that are attached to it
	// that's how magic works, I guess?
	for (auto& go : *m_screen->getObjects(GameObjectType::_Spell)) {
		if (Spell* spell = dynamic_cast<Spell*>(go)) {
			if (spell->getOwner() == this) {
				spell->onOwnerDisposed();
			}
		}
	}
	m_attributes.currentHealthPoints = 0;
	m_isDead = true;
}

void LevelMovableGameObject::setFightAnimation(const sf::Time& animationTime, GameObjectState fightAnimation, bool isBlocking) {
	m_movingBehavior->setFightAnimation(animationTime, fightAnimation, isBlocking);
}

const Level* LevelMovableGameObject::getLevel() const {
	return m_level;
}

bool LevelMovableGameObject::isFacingRight() const {
	return m_movingBehavior->isFacingRight();
}

bool LevelMovableGameObject::isUpsideDown() const {
	return m_movingBehavior->isUpsideDown();
}

bool LevelMovableGameObject::isIgnoreDynamicTiles() const {
	return m_movingBehavior->isIgnoreDynamicTiles();
}

bool LevelMovableGameObject::isDead() const {
	return m_isDead;
}

bool LevelMovableGameObject::isImmortal() const {
	return m_isImmortal;
}

bool LevelMovableGameObject::isStunned() const {
	return m_stunnedTime > sf::Time::Zero;
}

bool LevelMovableGameObject::isFeared() const {
	return m_fearedTime > sf::Time::Zero;
}

void LevelMovableGameObject::loadBehavior() {
	delete m_attackingBehavior;
	m_attackingBehavior = createAttackingBehavior();

	delete m_movingBehavior;
	m_movingBehavior = createMovingBehavior();
}

void LevelMovableGameObject::setReady() {
	m_movingBehavior->setReady();
}

void LevelMovableGameObject::flipGravity() {
	m_movingBehavior->flipGravity();
	m_animatedSprite.setFlippedY(m_movingBehavior->isUpsideDown());
}

GameObjectState LevelMovableGameObject::getState() const {
	return m_state;
}

SpellManager* LevelMovableGameObject::getSpellManager() const {
	return m_spellManager;
}

MovingBehavior* LevelMovableGameObject::getMovingBehavior() const {
	return m_movingBehavior;
}

AttackingBehavior* LevelMovableGameObject::getAttackingBehavior() const {
	return m_attackingBehavior;
}

const AttributeData* LevelMovableGameObject::getAttributes() const {
	return &m_attributes;
}

void LevelMovableGameObject::consumeFood(const sf::Time& duration, const AttributeData& attributes) {
	if (m_foodAttributes.first > sf::Time::Zero) {
		// old food attributes have to be removed
		m_attributes.removeBean(m_foodAttributes.second);
	}
	m_foodAttributes = std::pair<sf::Time, AttributeData>(duration, attributes);
	m_attributes.addBean(attributes);
}

bool LevelMovableGameObject::isAlly() const {
	return false;
}

float LevelMovableGameObject::getConfiguredMaxVelocityX() const {
	return m_movingBehavior->getMaxVelocityX();
}

float LevelMovableGameObject::getConfiguredMaxVelocityYDown() const {
	return m_movingBehavior->getMaxVelocityYDown();
}

float LevelMovableGameObject::getConfiguredMaxVelocityYUp() const {
	return m_movingBehavior->getMaxVelocityYUp();
}

