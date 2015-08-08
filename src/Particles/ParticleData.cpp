#include "Particles/ParticleData.h"

namespace particles
{
    void ParticleData::generate(size_t maxSize)
    {
        count = maxSize;
        countAlive = 0;

        pos.reset(new sf::Vector2f[maxSize]);
		size.reset(new sf::Vector3f[maxSize]);
    	col.reset(new sf::Color[maxSize]);
        startCol.reset(new sf::Color[maxSize]);
    	endCol.reset(new sf::Color[maxSize]);
        vel.reset(new sf::Vector2f[maxSize]);
        acc.reset(new sf::Vector2f[maxSize]);
    	time.reset(new sf::Vector3f[maxSize]);
        alive.reset(new bool[maxSize]);
    }

    void ParticleData::kill(size_t id)
    {
        if (countAlive > 0)
        {
            alive[id] = false;
            swapData(id, countAlive - 1);
            countAlive--;
        }
    }

    void ParticleData::wake(size_t id)
    {
        if (countAlive < count)
        {
            alive[id] = true;
            swapData(id, countAlive);
            countAlive++;
        }
    }

    void ParticleData::swapData(size_t a, size_t b)
    {
        std::swap(pos[a], pos[b]);
		std::swap(size[a], size[b]);
        std::swap(col[a], col[b]);
        std::swap(startCol[a], startCol[b]);
        std::swap(endCol[a], endCol[b]);
        std::swap(vel[a], vel[b]);
        std::swap(acc[a], acc[b]);
        std::swap(time[a], time[b]);
        std::swap(alive[a], alive[b]);
    }
}
