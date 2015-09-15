#include "GUI/ProgressLog.h"

ProgressLog::ProgressLog()
{
}

ProgressLog::~ProgressLog()
{
}

void ProgressLog::update(const sf::Time& frameTime)
{
	auto& it = m_logTexts.begin();
	while (it != m_logTexts.end())
	{
		(*it).second = (*it).second - frameTime;
		if ((*it).second <= sf::Time::Zero)
		{
			it = m_logTexts.erase(it);
			calculatePositions();
		}
		else
		{
			it++;
		}
	}
}

void ProgressLog::render(sf::RenderTarget& renderTarget)
{
	for (auto& it : m_logTexts)
	{
		renderTarget.draw(it.first);
	}
}

void ProgressLog::addItemProgress(const std::string& itemID, int amount)
{
	BitmapText progress;
	progress.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	progress.setColor(amount < 0 ? sf::Color::Red : sf::Color::Green);
	std::wstring text = std::to_wstring(amount) + L"x ";
	text.append(itemID.compare("gold") == 0 ? g_textProvider->getText("Gold") : g_textProvider->getText(itemID));
	progress.setString(text);
	m_logTexts.push_back(std::make_pair(progress, TIME_TO_LIVE));
	calculatePositions();
}

void ProgressLog::addQuestProgress(const std::string& questID)
{
	// TBD
	BitmapText progress;
	progress.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	progress.setColor(sf::Color::Green);
	progress.setString(g_textProvider->getText(questID));
	m_logTexts.push_back(std::make_pair(progress, TIME_TO_LIVE));
	calculatePositions();
}

void ProgressLog::calculatePositions()
{
	float yOffset = YOFFSET;
	for (auto& it : m_logTexts)
	{
		it.first.setPosition(sf::Vector2f(
			WINDOW_WIDTH - it.first.getLocalBounds().width - XOFFSET, yOffset));
		yOffset += 2.5f * GUIConstants::CHARACTER_SIZE_M;
	}
}