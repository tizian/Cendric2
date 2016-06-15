#include "Screens/SplashScreen.h"
#include "Screens/ScreenManager.h"

using namespace particles;

const std::string SPRITE_PATH_BG = "res/assets/screens/screen_splash_bg.png";
const std::string SPRITE_PATH_FG = "res/assets/screens/screen_splash_fg.png";

SplashScreen::SplashScreen() : Screen(nullptr) {
	g_resourceManager->loadTexture(SPRITE_PATH_BG, ResourceType::Unique, this);
	g_resourceManager->loadTexture(SPRITE_PATH_FG, ResourceType::Global);
	m_screenSpriteBackground = sf::Sprite((*g_resourceManager->getTexture(SPRITE_PATH_BG)));
	m_screenSpriteForeground = sf::Sprite((*g_resourceManager->getTexture(SPRITE_PATH_FG)));

	g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_FLAME)->setSmooth(true);
	m_ps_right = new particles::TextureParticleSystem(1000, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_FLAME));
	m_ps_left = new particles::TextureParticleSystem(1000, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_FLAME));
	loadFireParticles(m_ps_left, sf::Vector2f(155.f, 330.f));
	loadFireParticles(m_ps_right, sf::Vector2f(1130.f, 330.f));
}

SplashScreen::~SplashScreen() {
	delete m_ps_left;
	delete m_ps_right;
}

void SplashScreen::execOnEnter(const Screen* previousScreen) {
	// add version nr
	m_versionText.setString("Cendric v" + std::string(CENDRIC_VERSION_NR));
	m_versionText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_versionText.setColor(COLOR_WHITE);
	m_versionText.setPosition(
		(WINDOW_WIDTH - m_versionText.getLocalBounds().width) / 2,
		WINDOW_HEIGHT - 18.f);
}

void SplashScreen::execUpdate(const sf::Time& frameTime) {
	if (g_inputController->isKeyActive(Key::Confirm) || g_inputController->isMouseJustPressedLeft()) {
		MenuScreen* menuScreen = new MenuScreen(nullptr);
		menuScreen->setFireParticles(m_ps_left, m_ps_right);
		m_ps_left = nullptr;
		m_ps_right = nullptr;
		setNextScreen(menuScreen);
		return;
	}
	if (g_inputController->isKeyActive(Key::Escape)) {
		m_screenManager->requestQuit();
		return;
	}
	updateObjects(GameObjectType::_Interface, frameTime);
	m_ps_left->update(frameTime);
	m_ps_right->update(frameTime);
}

void SplashScreen::render(sf::RenderTarget& renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_screenSpriteBackground);
	m_ps_left->render(renderTarget);
	m_ps_right->render(renderTarget);
	renderTarget.draw(m_screenSpriteForeground);
	renderObjects(GameObjectType::_Interface, renderTarget);
	renderTarget.draw(m_versionText);
}

void SplashScreen::execOnExit(const Screen* nextScreen) {
	g_resourceManager->deleteUniqueResources(this);
}

void SplashScreen::loadFireParticles(particles::TextureParticleSystem* ps, const sf::Vector2f& center) {
	ps->additiveBlendMode = true;
	ps->emitRate = 70.f;

	// Generators
	auto spawner = ps->addSpawner<particles::BoxSpawner>();
	spawner->center = center;
	spawner->size = sf::Vector2f(65.f, 0.f);

	auto sizeGen = ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 60.f;
	sizeGen->maxStartSize = 100.f;
	sizeGen->minEndSize = 40.f;
	sizeGen->maxEndSize = 80.f;

	auto colGen = ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(171, 105, 243);
	colGen->maxStartCol = sf::Color(171, 105, 243);
	colGen->minEndCol = sf::Color(77, 54, 130, 200);
	colGen->maxEndCol = sf::Color(77, 54, 130, 200);

	auto velGen = ps->addGenerator<particles::AimedVelocityGenerator>();
	velGen->goal = center - (sf::Vector2f(0.f, 200.f));
	velGen->minStartSpeed = 60.f;
	velGen->maxStartSpeed = 100.f;

	auto timeGen = ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 0.5f;
	timeGen->maxTime = 1.8f;

	// Updaters
	ps->addUpdater<particles::TimeUpdater>();
	ps->addUpdater<particles::ColorUpdater>();
	ps->addUpdater<particles::EulerUpdater>();
	ps->addUpdater<particles::SizeUpdater>();
}