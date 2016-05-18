#pragma once

#include "global.h"

enum class DamageNumberType {
	Damage,
	DamageOverTime,
	Heal,
	HealOverTime
};

class BitmapText;

struct DamageNumberData {
	bool active;
	float startPosition;
	float time;
	BitmapText* text;
};

class DamageNumbers {
public:
	DamageNumbers(bool isAlly);
	~DamageNumbers();

	void update(const sf::Time& frameTime);
	void render(sf::RenderTarget& target);

	void emitNumber(int value, sf::Vector2f& position, DamageNumberType type);

private:
	static const int MAX_NUMBERS;
	static const float OFFSET;
	static const float DISTANCE;
	static const float TIME;

	std::vector<DamageNumberData> m_data;
	int m_nextIndex;
	bool m_isAlly;
};