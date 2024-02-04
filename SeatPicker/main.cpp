#include <SFML/Graphics.hpp>
#include <ctime>
#include "Classroom.h"
#include "FileManager.h"

#define SQUARE_HORIZONTAL 120
#define SQUARE_VERTICAL 80

enum {MAIN_SCREEN, RANDOMIZER, RANDOMIZED};

sf::Font font;

int main() {
	//initializing
	font.loadFromFile(string("C:/Windows/Fonts/malgun.ttf"));
	srand((unsigned int)time(NULL));
	////
	
	sf::RenderWindow window
		(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), L"ÀÚ¸® »Ì±â");

	//window initializing
	window.setFramerateLimit(30);
	////
	
	int currentScreen = MAIN_SCREEN;

	FileManager fm;
	Classroom cr;
	vector<sf::Drawable*> drawables;

	byte2d* layout = nullptr;
	list<Seat>* randomized = nullptr;
	vector<wstring>* names = nullptr;

	while (window.isOpen()) {
		sf::Event event;

		switch (currentScreen) {
		case MAIN_SCREEN: {
			//highlights where cursor is
			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

			if (mousePosition.x >= 0 && mousePosition.x <= 375) {
				sf::RectangleShape highlight(sf::Vector2f(375, 30));
				highlight.setFillColor(sf::Color(0xE5, 0xF3, 0xFF));
				highlight.setPosition(0, mousePosition.y - mousePosition.y % 30 + 3);

				drawables.push_back(new sf::RectangleShape(highlight));
			}

			//shows list of files
			vector<sf::Text>* files = fm.getSeatFileList();

			for (auto& file : *files) {
				file.setColor(sf::Color::Black);
				drawables.push_back(new sf::Text(file));
			}

			delete files;
			break;
		}

		case RANDOMIZER:
		case RANDOMIZED: {
			//'back' button
			drawables.push_back(new sf::RectangleShape(sf::Vector2f(50, 50)));

			//something went wrong
			if (layout == nullptr) {
				cout << "nptr";
				break;
			}

			//shows layout of desks in classroom
			vector<byte>& seats = layout->arr;
			
			for (size_t i = 0; i < seats.size(); i++) {
				sf::RectangleShape square(sf::Vector2f(SQUARE_HORIZONTAL - 4, SQUARE_VERTICAL - 4));

				float x = 50 + SQUARE_HORIZONTAL * (i % layout->sizeOfRow);
				float y = 100 + SQUARE_VERTICAL * (int)(i / layout->sizeOfRow);

				square.setPosition(sf::Vector2f(x, y));
				square.setFillColor(sf::Color::White);
				square.setOutlineColor(seats[i] ? sf::Color::Black : sf::Color::White);
				square.setOutlineThickness(2.0);

				drawables.push_back(new sf::RectangleShape(square));
			}
			
			//if randomizing is not done yet
			if (randomized == nullptr || names == nullptr) break;

			//shows names
			int i = 0;
			
			for (auto iter = randomized->begin(); iter != randomized->end() && names->size() > i; iter++) {
				sf::Text name((*names)[i++], font, 25);
				name.setPosition(sf::Vector2f(58 + iter->x * SQUARE_HORIZONTAL, 120 + iter->y * SQUARE_VERTICAL));
				name.setFillColor(sf::Color::Black);
				drawables.push_back(new sf::Text(name));
			}
		}

		default:
			break;
		}

		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::MouseButtonReleased: {
				sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

				switch (currentScreen) {
				case MAIN_SCREEN: {
					if (mousePosition.x <= 375) {
						string fileName = fm.getSeatFileName(mousePosition.y / 30);
						if (fileName == "") break;

						layout = fm.getSeatSquares(fileName);
						cr.setLayout(layout);

						currentScreen = RANDOMIZER;
					}
					break;
				}

				case RANDOMIZER: 
				case RANDOMIZED:
					//back to main screen
					if (mousePosition.x <= 50 && mousePosition.y <= 50) {
						delete layout;
						delete randomized;
						delete names;
						layout = nullptr;
						randomized = nullptr;
						names = nullptr;

						currentScreen = MAIN_SCREEN;
						break;
					}

					if (currentScreen == RANDOMIZED) cr.setLayout(layout);

					//activating randomizer
					randomized = cr.randomize();
					names = fm.getNames();
					currentScreen = RANDOMIZED;

					break;

				default:
					break;
				}
				break;
			}

			default:
				break;
			}

		}

		window.clear(sf::Color::White);

		for (const auto& drawable : drawables) {
			window.draw(*drawable);
		}

		//clearing drawables
		for (const auto& drawable : drawables) {
			delete drawable;
		}
		drawables.clear();

		window.display();
	}

	for (const auto& drawable : drawables) {
		delete drawable;
	}
	delete layout;
	delete randomized;

	return 0;
}