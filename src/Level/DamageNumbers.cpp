#include "Level/DamageNumbers.h"
#include "GUI/BitmapText.h"
#include "GUI/GUIConstants.h"
#include "TextProvider.h"


const int DamageNumbers::MAX_NUMBERS = 5;
const float DamageNumbers::START_OFFSET = -15.f;
const float DamageNumbers::DISTANCE = -40.f;
const float DamageNumbers::TIME = 0.8f;

DamageNumbers::DamageNumbers(bool isAlly) {
	m_isAlly = isAlly;
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
	offset = START_OFFSET;

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

void DamageNumbers::emitNumber(int value, const sf::Vector2f& position, DamageNumberType type, bool critical) {
	emitString(std::to_string(value), position, type);
	if (critical) {
		// Force "Critical!" text to be CHARACTER_SIZE_M
		if (type == DamageNumberType::Damage) type = DamageNumberType::DamageOverTime;
		if (type == DamageNumberType::Heal) type = DamageNumberType::HealOverTime;
		std::string criticalText = g_textProvider->getText("Critical");
		criticalText.push_back('!');
		emitString(criticalText, position, type);
	}
}

void DamageNumbers::emitString(std::string str, const sf::Vector2f& position, DamageNumberType type) {
	DamageNumberData& data = m_data[m_nextIndex];

	data.active = true;
	data.time = 0.f;
	data.text->setString(str);

	float characterSize;
	if (type == DamageNumberType::DamageOverTime || type == DamageNumberType::HealOverTime) {
		characterSize = GUIConstants::CHARACTER_SIZE_M;
	}
	else {
		characterSize = GUIConstants::CHARACTER_SIZE_L;
	}
	data.text->setCharacterSize(characterSize);
	offset -= 1.5f * characterSize;

	const sf::FloatRect& rect = data.text->getBounds();
	sf::Vector2f startPosition = sf::Vector2f(position.x - 0.5f * rect.width, position.y + offset);
	data.text->setPosition(startPosition);
	data.startPosition = startPosition.y;

	if (type == DamageNumberType::Damage || type == DamageNumberType::DamageOverTime) {
		if (m_isAlly) {
			data.text->setColor(COLOR_DAMAGE_ALLY);
		}
		else {
			data.text->setColor(COLOR_DAMAGE_ENEMY);
		}
	}
	else {
		if (m_isAlly) {
			data.text->setColor(COLOR_HEAL_ALLY);
		}
		else {
			data.text->setColor(COLOR_HEAL_ENEMY);
		}
	}

	m_nextIndex++;
	if (m_nextIndex > MAX_NUMBERS - 1) {
		m_nextIndex = 0;
	}
}