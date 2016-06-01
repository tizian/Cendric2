#include "Spells/BoomerangSpell.h"

const sf::Time BoomerangSpell::DAMAGE_INTERVAL = sf::seconds(2.f);
const float BoomerangSpell::ROTATION_PER_S = 2 * M_TWOPI;

void BoomerangSpell::load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	init(data);
	Spell::load(data, mob, target);

	// ellipse params
	m_a = 0.5f * std::min(dist(mob->getCenter(), target) + 50.f, data.range);
	m_b = 0.5f * m_a;
	m_theta = std::atan2(m_velocity.y, m_velocity.x);
	m_u = M_PI * std::sqrt(2 * (m_a * m_a + m_b * m_b));
	m_center = m_mob->getCenter() + sf::Vector2f(m_mob->isFacingRight() ? m_a : -m_a, 0.f);
	m_direction = m_mob->isFacingRight() ? -1 : 1;
	m_t = m_mob->isFacingRight() ? M_TWOPI : 0.f;
}

void BoomerangSpell::init(const SpellData& data) {
	setSpriteOffset(sf::Vector2f(-30.f, -5.f));

	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_boomerang));
	spellAnimation->addFrame(sf::IntRect(0, data.skinNr * 30, 90, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(false);
}

void BoomerangSpell::update(const sf::Time& frameTime) {
	if (m_isDisposed) return;

	// update rotation of sprite
	m_currentRotation += frameTime.asSeconds() * ROTATION_PER_S;
	if (m_currentRotation > M_TWOPI) m_currentRotation -= M_TWOPI;

	// update t
	m_t += m_direction * frameTime.asSeconds() * m_data.speed * M_TWOPI / m_u;
	if (m_t > M_TWOPI || m_t < 0.f) {
		setDisposed();
	}

	// damage
	updateTime(m_damageTimeout, frameTime);
	if (m_damageTimeout == sf::Time::Zero) {
		if (!m_data.isAlly) {
			// check collisions with allies
			if (checkCollisionsWithAllies(getBoundingBox())) {
				m_damageTimeout = DAMAGE_INTERVAL;
				m_isDisposed = false;
			}
		}
		else {
			// check collisions with enemies
			if (checkCollisionsWithEnemies(getBoundingBox())) {
				m_damageTimeout = DAMAGE_INTERVAL;
				m_isDisposed = false;
			}
		}
	}

	// ellipse trajectory
	sf::Vector2f position = m_center + sf::Vector2f(
		m_a * std::cos(m_t + M_PI) * std::cos(m_theta) - m_b * std::sin(m_t + M_PI) * std::sin(m_theta),
		m_a * std::cos(m_t + M_PI) * std::sin(m_theta) + m_b * std::sin(m_t + M_PI) * std::cos(m_theta)
	);

	setPosition(position);
	AnimatedGameObject::update(frameTime);

	updateTime(m_data.activeDuration, frameTime);
	if (m_data.activeDuration == sf::Time::Zero) {
		setDisposed();
	}

	setSpriteRotation(m_currentRotation);
}

void BoomerangSpell::onOwnerDisposed() {
	setDisposed();
	m_mob = nullptr;
}

void BoomerangSpell::setDisposed() {
	Spell::setDisposed();
	m_mob->setReady();
}


