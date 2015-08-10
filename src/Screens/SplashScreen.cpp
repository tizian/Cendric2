#include "Screens/SplashScreen.h"

using namespace std;

SplashScreen::SplashScreen() : Screen(nullptr)
{
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_screen_splash)));
}

void SplashScreen::execOnEnter(const Screen* previousScreen)
{
	// add burning fire baskets
	FireBasket* fireBasket1 = new FireBasket();
	FireBasket* fireBasket2 = new FireBasket();
	fireBasket1->setPosition(sf::Vector2f(60.f, 140.f));
	fireBasket2->setPosition(sf::Vector2f(998.f, 140.f));
	addObject(GameObjectType::_Undefined, fireBasket1);
	addObject(GameObjectType::_Undefined, fireBasket2);

	SpellSlot *slotInactive = new SpellSlot(sf::Vector2f(200, 100), DEFAULT_FIREBALL, false);
	addObject(GameObjectType::_Undefined, slotInactive);

	SpellSlot *slotActive = new SpellSlot(sf::Vector2f(400, 100), DEFAULT_FIREBALL, true);
	slotActive->playAnimation(DEFAULT_FIREBALL.cooldown);
	addObject(GameObjectType::_Undefined, slotActive);

	// Particle System Test
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(10000, g_resourceManager->getTexture(ResourceID::Texture_Particle_blob)));
	m_ps->setAdditiveBlendMode(true);
	m_ps->emitRate = 10000.0f / 5.0f;

	// Generators
	auto posGen = m_ps->addGenerator<particles::BoxPositionGenerator>();
	posGen->center = sf::Vector2f(640, 720);
	posGen->size = sf::Vector2f(20.0f, 5.0f);

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 3.0f;
	sizeGen->maxStartSize = 12.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 2.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(150, 0, 180, 255);
	colGen->maxStartCol = sf::Color(220, 255, 220, 255);
	colGen->minEndCol = sf::Color(128, 0, 150, 0);
	colGen->maxEndCol = sf::Color(180, 128, 220, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = -20.f;
	velGen->maxAngle = 20.f;
	velGen->minStartVel = 100.f;
	velGen->maxStartVel = 100.0f;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 1.0f;
	timeGen->maxTime = 5.0f;

	// Updaters
	auto timeUpdater = m_ps->addUpdater<particles::TimeUpdater>();

	auto colorUpdater = m_ps->addUpdater<particles::ColorUpdater>();

	//auto attractorUpdater = m_ps->addUpdater<particles::AttractorUpdater>();
	//attractorUpdater->add(sf::Vector3f(640, 360, 1000.0f));

	auto eulerUpdater = m_ps->addUpdater<particles::EulerUpdater>();
	//eulerUpdater->globalAcceleration = sf::Vector2f(0.0f, 1000.0f);
}

Screen* SplashScreen::update(const sf::Time& frameTime)
{
	if (g_inputController->isKeyActive(Key::Escape) || g_inputController->isMouseJustPressedLeft())
	{
		return new MenuScreen(nullptr);
	}
	updateObjects(GameObjectType::_Undefined, frameTime);
	m_ps->update(frameTime);
	return this;
}

void SplashScreen::render(sf::RenderTarget &renderTarget)
{
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_screenSprite);
	renderObjects(GameObjectType::_Undefined, renderTarget);
	m_ps->render(renderTarget);
}

void SplashScreen::execOnExit(const Screen *nextScreen)
{
	g_resourceManager->deleteResource(ResourceID::Texture_screen_splash);
}