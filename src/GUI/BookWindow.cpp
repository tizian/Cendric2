#include "GUI/BookWindow.h"
#include <map>

const float BookWindow::WIDTH = WINDOW_WIDTH / 3.f;
const float BookWindow::HEIGHT = WINDOW_HEIGHT - 2 * GUIConstants::TOP;

BookWindow::BookWindow(const BookData& data, MapScreen* screen) : Window(
	sf::FloatRect(0.f, 0.f, WIDTH, HEIGHT),
	WindowOrnamentStyle::FANCY,
	COLOR_LIGHT_BROWN, // main
	COLOR_TRANSPARENT, // back
	COLOR_DARK_BROWN) // ornament 
{
	m_data = data;

	std::map<char, int> charMap;

	charMap.insert(std::make_pair('A', 0));

	m_leftArrow = new ArrowButton(false);
	m_leftArrow->setMainColor(COLOR_DARK_BROWN);
	m_leftArrow->setMouseoverColor(COLOR_WHITE);

	m_rightArrow = new ArrowButton(true);
	m_rightArrow->setMainColor(COLOR_DARK_BROWN);
	m_rightArrow->setMouseoverColor(COLOR_WHITE);

	if (m_data.title.empty() && m_data.pages.empty()) {
		// this book has no pages. let's give it one.
		BookPage page;
		m_data.pages.push_back(page);
	}

	// select first page
	if (m_data.title.empty()) {
		setPage(0);
	}
	else {
		// Init book title
		m_bookTitle.setCharacterSize(24);
		m_bookTitle.setColor(COLOR_DARK_BROWN);
		m_bookTitle.setTextStyle(TextStyle::Shadowed);
		m_bookTitle.setString(g_textProvider->getText(m_data.title, "book"));
		m_bookTitle.setTextAlignment(TextAlignment::Center);

		setPage(-1);
	}

	m_title.setCharacterSize(GUIConstants::CHARACTER_SIZE_L);
	m_title.setColor(COLOR_DARK_BROWN);

	m_content.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_content.setColor(COLOR_BLACK);

	setPosition(sf::Vector2f(0.5f * (WINDOW_WIDTH - WIDTH), 0.5f * (WINDOW_HEIGHT - HEIGHT)));
}

BookWindow::~BookWindow() {
	delete m_leftArrow;
	delete m_rightArrow;
	g_resourceManager->deleteResource(ResourceID::Sound_page);
}

void BookWindow::render(sf::RenderTarget& renderTarget) {
	Window::render(renderTarget);
	if (m_currentPage == -1) {
		renderTarget.draw(m_bookTitle);
	}
	else {
		renderTarget.draw(m_title);
		renderTarget.draw(m_content);
	}

	m_leftArrow->render(renderTarget);
	m_rightArrow->render(renderTarget);
}

bool BookWindow::updateWindow(const sf::Time frameTime) {
	Window::update(frameTime);
	if (g_inputController->isKeyJustPressed(Key::Escape)) {
		g_inputController->lockAction();
		return false;
	}

	m_leftArrow->update(frameTime);
	m_rightArrow->update(frameTime);

	if (g_inputController->isKeyJustPressed(Key::Left) || m_leftArrow->isClicked()) {
		g_inputController->lockAction();
		setPage(m_currentPage - 1);
	}
	else if (g_inputController->isKeyJustPressed(Key::Right) || m_rightArrow->isClicked()) {
		g_inputController->lockAction();
		setPage(m_currentPage + 1);
	}

	return true;
}

void BookWindow::setPage(int index) {
	g_resourceManager->playSound(m_sound, ResourceID::Sound_page);
	if (m_data.title.empty() && index < 0) return;
	if (index < -1 || index + 1 > static_cast<int>(m_data.pages.size())) return;
	m_currentPage = index;
	m_rightArrow->setEnabled(m_currentPage + 1 < static_cast<int>(m_data.pages.size()));
	m_leftArrow->setEnabled((!m_data.title.empty() && m_currentPage > -1) || m_currentPage > 0);

	if (m_currentPage >= 0) {
		m_title.setString(g_textProvider->getText(m_data.pages.at(m_currentPage).title, "book"));
		m_content.setString(g_textProvider->getCroppedText(m_data.pages.at(m_currentPage).content, "book",
			GUIConstants::CHARACTER_SIZE_M, static_cast<int>(WIDTH - 2 * GUIConstants::LEFT + 10)));

		m_title.setPosition(getPosition() + sf::Vector2f(0.5f * WIDTH - 0.5f * m_title.getLocalBounds().width, GUIConstants::TOP));
		m_content.setPosition(
			getPosition() + sf::Vector2f(GUIConstants::LEFT,
				m_title.getString().empty() ? GUIConstants::TOP : GUIConstants::TOP + 3 * GUIConstants::CHARACTER_SIZE_L));
	}
}

void BookWindow::setPosition(const sf::Vector2f& pos) {
	Window::setPosition(pos);

	m_leftArrow->setPosition(pos + sf::Vector2f(0.5f * WIDTH - 20.f - m_leftArrow->getBoundingBox()->width, HEIGHT - 50.f));
	m_rightArrow->setPosition(pos + sf::Vector2f(0.5f * WIDTH + 20.f, HEIGHT - 50.f));

	m_bookTitle.setPosition(pos + sf::Vector2f(0.5f * WIDTH - 0.5f * m_bookTitle.getLocalBounds().width, 0.3f * HEIGHT));
	m_title.setPosition(pos + sf::Vector2f(0.5f * WIDTH - 0.5f * m_title.getLocalBounds().width, GUIConstants::TOP));
	m_content.setPosition(
		pos + sf::Vector2f(GUIConstants::LEFT, 
			m_title.getString().empty() ? GUIConstants::TOP : GUIConstants::TOP + 3 * GUIConstants::CHARACTER_SIZE_L));
}