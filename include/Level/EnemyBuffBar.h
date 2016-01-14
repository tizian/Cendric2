#pragma once

#include "global.h"
#include "Structs/DamageOverTimeData.h"
#include "AnimatedSprite.h"
#include "Enums/DamageType.h"
#include "ResourceManager.h"

class Enemy;

// the buff bar that displays debuffs (maybe later buffs too) on an enemy
class EnemyBuffBar {
public:
	EnemyBuffBar(Enemy* enemy);
	~EnemyBuffBar();

	void addFeared(const sf::Time& duration);
	void addStunned(const sf::Time& duration);
	void addDotBuff(const sf::Time& duration, DamageType type);
	void clear();

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

private:
	std::vector<std::pair<sf::Time, AnimatedSprite>> m_buffs;
	Enemy* m_enemy;

	void calculateBuffPositions();

	static const float BUFFBAR_OFFSET_Y;
	static const float BUFF_SIZE;
	Animation m_fearAnimation;
	Animation m_stunAnimation;
	std::map<DamageType, Animation> m_dotAnimations;

	// initialize the animations
	inline void initAnimations() {
		m_stunAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_debuff_stun));
		m_stunAnimation.addFrame(sf::IntRect(0, 0, 25, 25));
		m_stunAnimation.addFrame(sf::IntRect(25, 0, 25, 25));
		m_fearAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_debuff_fear));
		m_fearAnimation.addFrame(sf::IntRect(0, 0, 25, 25));
		m_fearAnimation.addFrame(sf::IntRect(25, 0, 25, 25));

		Animation dotAnimation;
		dotAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_damageTypes));
		sf::IntRect textureLocation(0, 50, 25, 25);

		for (int dmgType = static_cast<int>(DamageType::VOID) + 1; dmgType < static_cast<int>(DamageType::MAX); dmgType++) {
			textureLocation.left = (dmgType - 1) * 25;
			dotAnimation.addFrame(textureLocation);
			m_dotAnimations.insert({ static_cast<DamageType>(dmgType), dotAnimation });
			dotAnimation.clearFrames();
		}
	}
};