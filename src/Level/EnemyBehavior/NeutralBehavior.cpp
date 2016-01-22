#include "Level/EnemyBehavior/NeutralBehavior.h"

EnemyAttitude NeutralBehavior::getAttitude() const {
	return EnemyAttitude::Neutral;
}

sf::Color NeutralBehavior::getConfiguredHealthColor() const {
	return sf::Color::Yellow;
}

void NeutralBehavior::updateAggro() {
	// TODO: check for stealing cendric.
}