#include "IceStaff.h"

IceStaff::IceStaff()
{
	load();
}

IceStaff::~IceStaff()
{
	g_resourceManager->deleteResource(ResourceID::Texture_weapon_icestaff);
}

void IceStaff::load()
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0, 0, 120, 120));

	Animation walkingAnimation;
	walkingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_weapon_icestaff));
	walkingAnimation.addFrame(sf::IntRect(0, 0, 120, 120));
	walkingAnimation.addFrame(sf::IntRect(120, 0, 120, 120));
	walkingAnimation.addFrame(sf::IntRect(240, 0, 120, 120));
	walkingAnimation.addFrame(sf::IntRect(360, 0, 120, 120));
	walkingAnimation.addFrame(sf::IntRect(480, 0, 120, 120));
	walkingAnimation.addFrame(sf::IntRect(600, 0, 120, 120));
	walkingAnimation.addFrame(sf::IntRect(720, 0, 120, 120));
	walkingAnimation.addFrame(sf::IntRect(840, 0, 120, 120));

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_weapon_icestaff));
	idleAnimation.addFrame(sf::IntRect(960, 0, 120, 120));
	idleAnimation.addFrame(sf::IntRect(1080, 0, 120, 120));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation jumpingAnimation;
	jumpingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_weapon_icestaff));
	jumpingAnimation.addFrame(sf::IntRect(1200, 0, 120, 120));
	jumpingAnimation.addFrame(sf::IntRect(1320, 0, 120, 120));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation fightingAnimation;
	fightingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_weapon_icestaff));
	fightingAnimation.addFrame(sf::IntRect(1440, 0, 120, 120));
	fightingAnimation.addFrame(sf::IntRect(1560, 0, 120, 120));
	fightingAnimation.addFrame(sf::IntRect(1680, 0, 120, 120));
	fightingAnimation.addFrame(sf::IntRect(1800, 0, 120, 120));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	setFrameTime(sf::seconds(0.08f));

	// initial values
	m_state = GameObjectState::Idle;
	m_isFacingRight = true;
	setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	playCurrentAnimation(true);
}
