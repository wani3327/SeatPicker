#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#define NOMINMAX

#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <io.h>
#include <SFML/Graphics.hpp>

#pragma warning(disable:4996)

using namespace std;

enum { NO_FILE };

extern sf::Font font;

struct byte2d {
	vector<byte> arr;
	size_t sizeOfRow;
};

class FileManager {
private:
	string programPath;

public:
	FileManager();

	vector<sf::Text>* getSeatFileList();
	string getSeatFileName(const int);
	byte2d* getSeatSquares(const string);
	vector<wstring>* getNames();
};

#endif // !FILEMANAGER_H