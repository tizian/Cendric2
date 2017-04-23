#pragma once

#include "global.h"
#include "GameObjectComponents/GameObjectComponent.h"
#include "Particles/ParticleSystem.h"

struct ParticleComponentData final {
	int particleCount = 0;
	float emitRate = 0;
	std::string texturePath;
	bool isAdditiveBlendMode = false;
	sf::RenderTexture* particleTexture = nullptr;
	particles::SizeGenerator* sizeGen = nullptr;
	particles::ColorGenerator* colorGen = nullptr;
	particles::ParticleGenerator* velGen = nullptr;
	particles::ParticleSpawner* spawner = nullptr;
	particles::TimeGenerator* timeGen = nullptr;
	particles::ColorUpdater* colorUpdater = nullptr;
	particles::EulerUpdater* eulerUpdater = nullptr;
};

// A game object component that holds a particle system
class ParticleComponent final : public GameObjectComponent {
public:
	ParticleComponent(const ParticleComponentData& data, GameObject* parent);
	~ParticleComponent();

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;
	void emitParticles(int particles);

	void flipOffsetX(bool flipped);
	void flipOffsetY(bool flipped);
	void setOffset(const sf::Vector2f& offset);
	void setVisible(bool visible);
	void setStatic(bool isStatic);
	void setEmitRate(float emitRate);
	void setPosition(const sf::Vector2f& pos) override;
	void setTexturePath(const std::string& texturePath);

	particles::ColorUpdater* getColorUpdater() const;
	particles::ColorGenerator* getColorGenerator() const;

	particles::TextureParticleSystem* getParticleSystem() const;

private:
	void loadParticleSystem();

private:
	ParticleComponentData m_data;
	particles::TextureParticleSystem* m_ps = nullptr;
	
	sf::Vector2f m_offset;
	bool m_isOffsetFlippedX = false;
	bool m_isOffsetFlippedY = false;

	bool m_isVisible = true;
	bool m_isStatic = false;
};