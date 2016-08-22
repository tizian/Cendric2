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
	AnimatedSprite sprite;
	sprite.setAnimation(&m_fearAnimation);
	m_buffs.push_back(std::pair<sf::Time, AnimatedSprite>(duration, sprite));
}

void EnemyBuffBar::addStunned(const sf::Time& duration) {
	AnimatedSprite sprite;
	sprite.setAnimation(&m_stunAnimation);
	m_buffs.push_back(std::pair<sf::Time, AnimatedSprite>(duration, sprite));
}

void EnemyBuffBar::addDotBuff(const sf::Time& duration, DamageType type) {
	AnimatedSprite sprite;
	sprite.setAnimation(&m_dotAnimations.at(type));
	m_buffs.push_back(std::pair<sf::Time, AnimatedSprite>(duration, sprite));
}

void EnemyBuffBar::render(sf::RenderTarget& target) {
	for (auto& buff : m_buffs) {
		target.draw(buff.second);
	}
}

void EnemyBuffBar::clear() {
	m_buffs.clear();
}

void EnemyBuffBar::update(const sf::Time& frameTime) {
	for (auto buffIt = m_buffs.begin(); buffIt != m_buffs.end(); /*don't increment here*/) {
		updateTime(buffIt->first, frameTime);
		buffIt->second.update(frameTime);
		if (buffIt->first <= sf::Time::Zero) {
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
		buff.second.setPosition(position);
		position.y -= BUFF_SIZE;
	}
}

