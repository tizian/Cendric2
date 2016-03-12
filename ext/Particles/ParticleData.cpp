#include "Particles/ParticleData.h"

namespace particles
{
    ParticleData::ParticleData(int maxSize) {
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
		texCoords.reset(new sf::IntRect[maxSize]);
		frame.reset(new int[maxSize]);
		frameTimer.reset(new float[maxSize]);
        alive.reset(new bool[maxSize]);
    }

    void ParticleData::kill(int id) {
        if (countAlive > 0) {
            alive[id] = false;
            swapData(id, countAlive - 1);
            countAlive--;
        }
    }

    void ParticleData::wake(int id) {
        if (countAlive < count) {
            alive[id] = true;
            swapData(id, countAlive);
            countAlive++;
        }
    }

    void ParticleData::swapData(int id1, int id2) {
        std::swap(pos[id1], pos[id2]);
		std::swap(size[id1], size[id2]);
        std::swap(col[id1], col[id2]);
        std::swap(startCol[id1], startCol[id2]);
        std::swap(endCol[id1], endCol[id2]);
        std::swap(vel[id1], vel[id2]);
        std::swap(acc[id1], acc[id2]);
        std::swap(time[id1], time[id2]);
		std::swap(texCoords[id1], texCoords[id2]);
		std::swap(frame[id1], frame[id2]);
		std::swap(frameTimer[id1], frameTimer[id2]);
        std::swap(alive[id1], alive[id2]);
    }
}
