#pragma once

#include "global.h"
#include "GUI/Window.h"
#include "GUI/BitmapText.h"
#include "GUI/GUIConstants.h"
#include "GUI/ArrowButton.h"
#include "Structs/BookData.h"

class MapScreen;

class BookWindow : public Window {
public:
	BookWindow(const BookData& data, MapScreen* screen);
	~BookWindow();

	void render(sf::RenderTarget& renderTarget) override;
	// returns true as long as the book is open and false as soon as the player closes it
	bool updateWindow(const sf::Time frameTime);
	void setPosition(const sf::Vector2f& pos) override;
	void setPage(int index);

	static const float WIDTH;
	static const float HEIGHT;

private:
	BookData m_data;

	ArrowButton* m_leftArrow;
	ArrowButton* m_rightArrow;

	BitmapText m_bookTitle;
	BitmapText m_title;
	BitmapText m_content;

	// -1 is the title page, may be empty.
	int m_currentPage = -1;
	MapScreen* m_screen;

	sf::Sound m_sound;
};