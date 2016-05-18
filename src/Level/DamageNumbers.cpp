#include "Level/DamageNumbers.h"
#include "GUI/BitmapText.h"

const int DamageNumbers::MAX_NUMBERS = 5;
const float DamageNumbers::OFFSET = -15.f;
const float DamageNumbers::DISTANCE = -40.f;
const float DamageNumbers::TIME = 0.8f;

DamageNumbers::DamageNumbers() {
	m_nextIndex = 0;
	for (int i = 0; i < MAX_NUMBERS; ++i) {
		DamageNumberData numberData;
		numberData.active = false;
		numberData.startPosition = 0.f;
		numberData.text = new BitmapText("", TextStyle::Shadowed, TextAlignment::Center);
		numberData.text->setCharacterSize(12);
		m_data.push_back(numberData);
	}
}

DamageNumbers::~DamageNumbers() {
	for (int i = 0; i < MAX_NUMBERS; ++i) {
		delete m_data[i].text;
	}
}

void DamageNumbers::update(const sf::Time& frameTime) {
	for (int i = 0; i < MAX_NUMBERS; ++i) {
		DamageNumberData& data = m_data[i];

		if (data.active) {
			data.time += frameTime.asSeconds();
			if (data.time < 0.8f * TIME) {
				sf::Vector2f pos = data.text->getPosition();
				pos.y = easeInOutQuad(data.time, data.startPosition, DISTANCE, 0.8f * TIME);
				data.text->setPosition(pos);
			}
			else if (data.time < TIME) {
				sf::Color col = data.text->getColor();
				col.a = static_cast<int>(linearTween(data.time - 0.8f * TIME, 255.f, -255.f, 0.2f * TIME));
				data.text->setColor(col);
			}
			else {
				data.active = false;
			}
		}
	}
}

void DamageNumbers::render(sf::RenderTarget& target) {
	for (int i = 0; i < MAX_NUMBERS; ++i) {
		DamageNumberData& data = m_data[i];
		if (data.active) {
			target.draw(*data.text);
		}
	}
}

void DamageNumbers::emitNumber(int value, sf::Vector2f& position, DamageNumberType type) {
	DamageNumberData& data = m_data[m_nextIndex];

	data.active = true;
	data.time = 0.f;
	data.text->setString(std::to_string(value));

	sf::FloatRect &rect = data.text->getBounds();
	sf::Vector2f startPosition = sf::Vector2f(position.x - 0.5f * rect.width, position.y + OFFSET);
	data.text->setPosition(startPosition);
	data.startPosition = startPosition.y;

	if (type == DamageNumberType::DamageOverTime || type == DamageNumberType::HealOverTime) {
		data.text->setCharacterSize(12);
	}
	else {
		data.text->setCharacterSize(16);
	}

	if (type == DamageNumberType::Damage || type == DamageNumberType::DamageOverTime) {
		data.text->setColor(COLOR_BAD);
	}
	else {
		data.text->setColor(COLOR_GOOD);
	}

	m_nextIndex++;
	if (m_nextIndex > MAX_NUMBERS - 1) {
		m_nextIndex = 0;
	}
}