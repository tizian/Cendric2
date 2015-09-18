#pragma once

#include "global.h"
#include "DynamicTile.h"
#include "LightObject.h"

class TorchTile : public DynamicTile, public LightObject
{
public:
	TorchTile(Level* level);
	void init() override;
	void load(int skinNr) override;
	void onHit(Spell* spell) override;

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;

	GameObjectType getConfiguredType() const override;

private:
};