#include "DynamicTiles/SimulatedWaterTile.h"
#include "Spell.h"

void SimulatedWaterTile::load(int skinNr)
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	m_isCollidable = false;

	// exercise 1a) initialize your tile here. The skin nr describes which tile was chosen inside the tilemap. Take a look at the Dropbox file (res / tilesets / dynamic tile tileset) for clarification.
	// the size of this simulated water tile is here already set, you can access it using getBoundingBox()
}

void SimulatedWaterTile::update(const sf::Time& frameTime)
{
	// use the super calls you need. But you'll need none, probably. onHit doesn't need it.
	//DynamicTile::update(frameTime);
	// exercise 1d) your simulation here.
}

void SimulatedWaterTile::render(sf::RenderTarget& target)
{
	// exercise 1f) render your simulation here.
}

void SimulatedWaterTile::onHit(Spell* spell)
{
	switch (spell->getConfiguredSpellID())
	{
	case SpellID::Chop:
		spell->setDisposed();
		// exercise 1b) create waves?^^
		break;
	case SpellID::Ice:
		// exercise 1c) check the spells position, split water into two tiles (if water was only one tile wide, (setDisposed)) etc
		spell->setDisposed();
		break;
	case SpellID::Fire:
		// waves?
		break;
	default:
		break;
	}
}