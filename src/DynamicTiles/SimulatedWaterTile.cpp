#include "DynamicTiles/SimulatedWaterTile.h"
#include "Spell.h"

using namespace std;

const float SimulatedWaterTile::TENSION = 0.3f;
const float SimulatedWaterTile::DAMPING = 0.05f;
const float SimulatedWaterTile::SPREAD = 0.5f;

void SimulatedWaterTile::load(int skinNr)
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	m_isCollidable = false;

	const sf::FloatRect *bb = getBoundingBox();
	m_x = getPosition().x;
	m_y = getPosition().y;
	m_width = bb->width;
	m_height = bb->height;
	m_nTiles = static_cast<int>(bb->width / 50.f); // Tilesize 50px

	float waterLevel = 40.f;		// configure
	int nColumsPerTile = 5;		// configure
	m_nColumns = nColumsPerTile * m_nTiles;
	m_columns = vector<WaterColumn>();
	for (int i = 0; i < m_nColumns; ++i)
	{
		WaterColumn c;
		c.targetHeight = waterLevel;
		c.height = waterLevel;
		c.velocity = 0.f;

		m_columns.push_back(c);
	}

	cout << "x: " << m_x << ", y: " << m_y << ", w: " << m_width << ", h: " << m_height << endl;

	m_leftDeltas = new float[m_nColumns];
	m_rightDeltas = new float[m_nColumns];
}

void SimulatedWaterTile::update(const sf::Time& frameTime)
{
	float dt = frameTime.asSeconds();
	dt *= 16.f;

	for (int i = 0; i < m_nColumns; ++i)
	{
		m_columns[i].update(DAMPING, TENSION, dt);
	}

	for (int iterations = 0; iterations < 8; ++iterations)
	{
		for (int i = 0; i < m_nColumns; ++i)
		{
			if (i > 0)
			{
				m_leftDeltas[i] = SPREAD * (m_columns[i].height - m_columns[i - 1].height);
				m_columns[i - 1].velocity += m_leftDeltas[i] * dt;
			}
			if (i < m_nColumns - 1)
			{
				m_rightDeltas[i] = SPREAD * (m_columns[i].height - m_columns[i + 1].height);
				m_columns[i + 1].velocity += m_rightDeltas[i] * dt;
			}
		}

		for (int i = 0; i < m_nColumns; ++i)
		{
			if (i > 0)
			{
				m_columns[i - 1].height += m_leftDeltas[i] * dt;
			}
			if (i < m_nColumns - 1)
			{
				m_columns[i + 1].height += m_rightDeltas[i] * dt;
			}
		}
	}
}

void SimulatedWaterTile::splash(float xPosition, float yPosition, float velocity)
{
	int index = static_cast<int>((xPosition - m_x) / (m_width / (m_nColumns - 1)));
	if (index > 0 && index < m_nColumns && yPosition > m_columns[index].height)
	{
		m_columns[index].velocity = velocity;
	}
}

void SimulatedWaterTile::render(sf::RenderTarget& target)
{
	sf::Color blue = sf::Color(25 * 0.8f, 64 * 0.8f, 127 * 0.8f, 0.5f * 255);
	sf::Color transparent = sf::Color(255, 255, 255, 0);

	int numQuads = m_nColumns - 1;
	float scale = m_width / (float)numQuads;

	sf::VertexArray quads(sf::Quads, 4 * numQuads);

	float thickness = 6.f;

	int quadIndex = 0;

	for (int i = 1; i < m_nColumns; ++i)
	{
		sf::Vector2f p1 = sf::Vector2f(m_x + (i - 1) * scale, m_y + m_height - m_columns[i - 1].height);
		sf::Vector2f p2 = sf::Vector2f(m_x + i * scale, m_y + m_height - m_columns[i].height);
		sf::Vector2f p3 = sf::Vector2f(p2.x, m_y + m_height);
		sf::Vector2f p4 = sf::Vector2f(p1.x, m_y + m_height);

		quads[quadIndex + 0].position = p1;
		quads[quadIndex + 0].color = blue;
		quads[quadIndex + 1].position = p2;
		quads[quadIndex + 1].color = blue;
		quads[quadIndex + 2].position = p3;
		quads[quadIndex + 2].color = blue;
		quads[quadIndex + 3].position = p4;
		quads[quadIndex + 3].color = blue;

		quadIndex += 4;
	}

	target.draw(quads);
}

void SimulatedWaterTile::onHit(Spell* spell)
{
	switch (spell->getConfiguredSpellID())
	{
	case SpellID::Ice:
		// exercise 1c) check the spells position, split water into two tiles (if water was only one tile wide, (setDisposed)) etc
		spell->setDisposed();
		break;
	default:
		float vx = spell->getVelocity().x;
		float vy = spell->getVelocity().y;
		float vel = std::sqrt(vx*vx + vy*vy);
		splash(spell->getPosition().x, spell->getPosition().y, -vel * 0.5f);
		spell->setDisposed();
		break;
	}
}