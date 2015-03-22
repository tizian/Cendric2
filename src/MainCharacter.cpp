#include "stdafx.h"

MainCharacter::MainCharacter() {}
MainCharacter::~MainCharacter() {}

void MainCharacter::load(std::string tag)
{
	Texture* cendricTexture = &(g_resourceManager->getTexture("main_character"));
	
	Animation walkingAnimationRight;
	walkingAnimationRight.setSpriteSheet(Texture(*cendricTexture));
	walkingAnimationRight.addFrame(sf::IntRect(0, 0, 80, 120));
	walkingAnimationRight.addFrame(sf::IntRect(80, 0, 80, 120));
	walkingAnimationRight.addFrame(sf::IntRect(160, 0, 80, 120));
	walkingAnimationRight.addFrame(sf::IntRect(240, 0, 80, 120));
	walkingAnimationRight.addFrame(sf::IntRect(320, 0, 80, 120));
	walkingAnimationRight.addFrame(sf::IntRect(400, 0, 80, 120));
	walkingAnimationRight.addFrame(sf::IntRect(480, 0, 80, 120));
	walkingAnimationRight.addFrame(sf::IntRect(560, 0, 80, 120));

	addAnimation("walking_right", walkingAnimationRight);

	Animation walkingAnimationLeft;
	walkingAnimationLeft.setSpriteSheet(Texture(*cendricTexture));
	walkingAnimationLeft.addFrame(sf::IntRect(0, 120, 80, 120));
	walkingAnimationLeft.addFrame(sf::IntRect(80, 120, 80, 120));
	walkingAnimationLeft.addFrame(sf::IntRect(160, 120, 80, 120));
	walkingAnimationLeft.addFrame(sf::IntRect(240, 120, 80, 120));
	walkingAnimationLeft.addFrame(sf::IntRect(320, 120, 80, 120));
	walkingAnimationLeft.addFrame(sf::IntRect(400, 120, 80, 120));
	walkingAnimationLeft.addFrame(sf::IntRect(480, 120, 80, 120));
	walkingAnimationLeft.addFrame(sf::IntRect(560, 120, 80, 120));

	addAnimation("walking_left", walkingAnimationLeft);
	setCurrentAnimation(getAnimation("walking_right"));

	m_animatedSprite.setFrameTime(sf::seconds(0.1));
	m_animatedSprite.setPosition(sf::Vector2f(800, 450));
}
