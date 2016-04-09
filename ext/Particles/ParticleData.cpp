#include "Particles/ParticleData.h"

namespace particles
{
    ParticleData::ParticleData(int maxSize) : count(maxSize), countAlive(0) {
        count = maxSize;
        countAlive = 0;

        pos = new sf::Vector2f[maxSize];
        size = new sf::Vector3f[maxSize];
        col = new sf::Color[maxSize];
        startCol = new sf::Color[maxSize];
        endCol = new sf::Color[maxSize];
        vel = new sf::Vector2f[maxSize];
        acc = new sf::Vector2f[maxSize];
        time = new sf::Vector3f[maxSize];
		texCoords = new sf::IntRect[maxSize];
		frame = new int[maxSize];
		frameTimer = new float[maxSize];
    }

    ParticleData::~ParticleData() {
        delete pos;
        delete size;
        delete col;
        delete startCol;
        delete endCol;
        delete vel;
        delete acc;
        delete time;
        delete texCoords;
        delete frame;
        delete frameTimer;
    }

    void ParticleData::kill(int id) {
        if (countAlive > 0) {
            swapData(id, countAlive - 1);
            countAlive--;
        }
    }

    void ParticleData::wake(int id) {
        if (countAlive < count) {
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
    }
}
