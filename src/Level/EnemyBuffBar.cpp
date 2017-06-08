#include "Level/EnemyBuffBar.h"
#include "Level/Enemy.h"

const float EnemyBuffBar::BUFFBAR_OFFSET_Y = 10.f;
const float EnemyBuffBar::BUFF_SIZE = 25.f;

EnemyBuffBar::EnemyBuffBar(Enemy* enemy) {
	m_enemy = enemy;
	initAnimations();
}

EnemyBuffBar::~EnemyBuffBar() {
}

void EnemyBuffBar::addFeared(const sf::Time& duration) {
	addDebuff(EnemyBuffType::Fear, &m_fearAnimation, duration);
}

void EnemyBuffBar::addStunned(const sf::Time& duration) {
	addDebuff(EnemyBuffType::Stun, &m_stunAnimation, duration);
}

void EnemyBuffBar::addDotBuff(const sf::Time& duration, DamageType type) {
	EnemyBuffType buffType;
	switch (type) {
	case DamageType::Physical:
		buffType = EnemyBuffType::Physical;
		break;
	case DamageType::Fire:
		buffType = EnemyBuffType::Fire;
		break;
	case DamageType::Ice:
		buffType = EnemyBuffType::Ice;
		break;
	case DamageType::Shadow:
		buffType = EnemyBuffType::Shadow;
		break;
	case DamageType::Light:
		buffType = EnemyBuffType::Light;
		break;
	default:
		return;
	}

	addDebuff(buffType, &m_dotAnimations.at(type), duration);
}

void EnemyBuffBar::render(sf::RenderTarget& target) {
	for (auto& buff : m_buffs) {
		target.draw(buff.second.second);
	}
}

void EnemyBuffBar::addDebuff(EnemyBuffType debuffType, const Animation* animation, const sf::Time& time) {
	if (contains(m_buffs, debuffType)) {
		m_buffs.at(debuffType).first = sf::seconds(std::max(
			time.asSeconds(),
			m_buffs.at(debuffType).first.asSeconds()));
		return;
	}
	AnimatedSprite sprite;
	sprite.setAnimation(animation);
	m_buffs.insert({ debuffType, std::pair<sf::Time, AnimatedSprite>(time, sprite) });
}

void EnemyBuffBar::clear() {
	m_buffs.clear();
}

void EnemyBuffBar::update(const sf::Time& frameTime) {
	for (auto buffIt = m_buffs.begin(); buffIt != m_buffs.end(); /*don't increment here*/) {
		updateTime(buffIt->second.first, frameTime);
		buffIt->second.second.update(frameTime);
		if (buffIt->second.first <= sf::Time::Zero) {
			buffIt = m_buffs.erase(buffIt);
		}
		else {
			++buffIt;
		}
	}
	calculateBuffPositions();
}

void EnemyBuffBar::calculateBuffPositions() {
	if (m_buffs.empty()) return;
	
	const sf::FloatRect* bb = m_enemy->getBoundingBox();
	sf::Vector2f position(
		bb->left + bb->width / 2.f - BUFF_SIZE / 2.f,
		bb->top - m_enemy->getConfiguredDistanceToHPBar() - BUFFBAR_OFFSET_Y - BUFF_SIZE);
	for (auto& buff : m_buffs) {
		buff.second.second.setPosition(position);
		position.y -= BUFF_SIZE;
	}
}

