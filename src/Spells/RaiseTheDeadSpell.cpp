#include "Spells/RaiseTheDeadSpell.h"
#include "GameObjectComponents/LightComponent.h"
#include "Level/Enemy.h"
#include "ObjectFactory.h"

void RaiseTheDeadSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	setSpriteOffset(sf::Vector2f(-27.f, -27.f));

	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(bean.spritesheetPath));
	for (int i = 0; i < 16; ++i) {
		spellAnimation->addFrame(sf::IntRect(i * 64, bean.skinNr * 64, 64, 64));
	}

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	Spell::load(bean, mob, target);

	LightData lightData(sf::Vector2f(m_boundingBox.width / 2.f, m_boundingBox.height / 2.f), 100.f, 0.6f);
	addComponent(new LightComponent(lightData, this));
}

void RaiseTheDeadSpell::execOnHit(LevelMovableGameObject* target) {
	if (!target->isDead() || target->isAlly()) return;
	Enemy* enemy = dynamic_cast<Enemy*>(target);
	if (enemy == nullptr || enemy->isResurrected()) return;
	if (enemy->getMentalStrength() >= m_data.strength) {
		m_screen->setNegativeTooltip("NotEnoughStrength");
		setDisposed();
		return;
	}

	AttributeData attributes;
	attributes.damagePhysical = m_data.damage;
	attributes.damageFire = m_data.damage;
	attributes.damageIce = m_data.damage;
	attributes.damageLight = m_data.damage;
	attributes.damageShadow = m_data.damage;

	// add an allied copy of that mob.
	enemy->setResurrected();
	Enemy* copy = ObjectFactory::Instance()->createEnemy(enemy->getEnemyID(), m_level, m_screen);
	copy->load(enemy->getSkinNr());
	copy->setAlly(m_data.duration);
	copy->addAttributes(m_data.duration, attributes);
	copy->setPosition(enemy->getPosition());
	m_screen->addObject(copy);
	setDisposed();
}

bool RaiseTheDeadSpell::checkCollisionsWithEnemies(const sf::FloatRect* boundingBox) {
	// this method is overridden to guarantee that the spell only hits once
	// and that the iterator is not invalidated (we change the enemy vector size on the fly)
	for (auto& go : *m_enemies) {
		if (!go->isViewable()) continue;
		Enemy* enemy = dynamic_cast<Enemy*>(go);
		if (enemy != nullptr && fastIntersect(*enemy->getBoundingBox(), *boundingBox)) {
			enemy->onHit(this);
			return true;
		}
	}
	return false;
}

