#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Structs/LadderTileData.h"

class LadderTile : public LevelDynamicTile {
public:
	LadderTile(const LadderTileData& data, LevelScreen* levelScreen);
	void init() override;
	void loadAnimation(int skinNr) override;
	void render(sf::RenderTarget& target) override;
	void onHit(LevelMovableGameObject* mob) { /*nop*/ }
	void onHit(Spell* spell) override { /*nop*/ }
	void setPosition(const sf::Vector2f& position) override;

	// returns a feasable position of a game object relative to this ladder
	// when a GO starts climbing on a ladder, it starts at this position.
	float getClimbingPositionY(GameObject* object) const;

	static const int LADDER_STEP;

private:
	std::string getSpritePath() const override;
	std::vector<sf::Sprite> m_sprites;
	int m_size;
};