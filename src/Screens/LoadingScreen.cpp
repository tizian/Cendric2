#include "Screens/LoadingScreen.h"

using namespace std;

inline void load(LevelScreen* levelToLoad, MapScreen* mapToLoad, LoadingScreen* loadingScreen)
{
	if (loadingScreen == nullptr) return;
	if (levelToLoad != nullptr)
	{
		levelToLoad->load();
	}
	else if (mapToLoad != nullptr)
	{
		mapToLoad->load();
	}
	loadingScreen->setLoaded();
}

LoadingScreen::LoadingScreen(CharacterCore* core) : Screen(core)
{
	if (core->getData().isInLevel)
	{
		m_levelToLoad = new LevelScreen(core->getData().currentLevel, getCharacterCore());
	}
	else
	{
		m_mapToLoad = new MapScreen(core->getData().currentMap, getCharacterCore());
	}
}

Screen* LoadingScreen::update(const sf::Time& frameTime)
{
	m_phi += (frameTime.asSeconds() * VELOCITY) / RADIUS;
	if (m_phi > 2 * M_PI) m_phi = 0.f;
	m_posGen->center = CENTER + sf::Vector2f(cos(m_phi) * RADIUS, sin(m_phi) * RADIUS);
	m_ps->update(frameTime);

	if (!m_isLoaded) return this;

	m_thread.join();
	if (m_mapToLoad != nullptr) return m_mapToLoad;
	if (m_levelToLoad != nullptr)
	{
		m_levelToLoad->loadDynamicTiles();
		return m_levelToLoad;
	}

	return this;
}

void LoadingScreen::render(sf::RenderTarget &renderTarget)
{
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(*m_title);
	m_ps->render(renderTarget);
}

void LoadingScreen::execOnEnter(const Screen *previousScreen)
{
	// Particle System 
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(10000, g_resourceManager->getTexture(ResourceID::Texture_Particle_blob)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 10000.0f / 1.0f;

	// Generators
	auto posGen = m_ps->addGenerator<particles::PointPositionGenerator>();
	m_posGen = posGen.get();

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 3.0f;
	sizeGen->maxStartSize = 12.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 2.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	m_colorGen = colGen.get();
	colGen->minStartCol = sf::Color(0, 0, 0, 255);
	colGen->maxStartCol = sf::Color(0, 0, 0, 255);
	colGen->minEndCol = sf::Color(128, 0, 150, 0);
	colGen->maxEndCol = sf::Color(180, 128, 220, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = -180.f;
	velGen->maxAngle = 180.f;
	velGen->minStartVel = 200.f;
	velGen->maxStartVel = 300.f;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 0.1f;
	timeGen->maxTime = 0.2f;

	// Updaters
	auto timeUpdater = m_ps->addUpdater<particles::TimeUpdater>();

	auto colorUpdater = m_ps->addUpdater<particles::ColorUpdater>();

	auto eulerUpdater = m_ps->addUpdater<particles::EulerUpdater>();

	// title
	m_title = new BitmapText(g_textProvider->getText("Loading"));
	m_title->setCharacterSize(50);
	m_title->setPosition(sf::Vector2f((WINDOW_WIDTH - m_title->getLocalBounds().width) / 2.f, 50.f));

	m_thread = std::thread(load, m_levelToLoad, m_mapToLoad, this);
}

void LoadingScreen::setLoaded()
{
	m_isLoaded = true;
}

void LoadingScreen::execOnExit(const Screen *nextScreen)
{
	delete m_title;
}