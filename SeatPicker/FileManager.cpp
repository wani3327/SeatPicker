#include "FileManager.h"

#define MAX_HORIZONTAL 15

FileManager::FileManager() {
	TCHAR buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);
	programPath = buffer;
}

vector<sf::Text>* FileManager::getSeatFileList(void) {
	long fileHandle;
	_finddata_t fileInfo;

	vector<sf::Text>* files = new vector<sf::Text>;

	try {
		if ((fileHandle = _findfirst((programPath + "/seat/*.txt").c_str(), &fileInfo)) == -1) {
			sf::Text file("No file", font);
			file.setPosition(sf::Vector2f(0, 0));

			files->push_back(file);
			throw(NO_FILE);
		}

		int i = 0;
		do {
			wchar_t unicodeName[MAX_PATH];

			int lenA = strlen(fileInfo.name);
			int lenU = ::MultiByteToWideChar(CP_ACP, 0, fileInfo.name, lenA, 0, 0);
			::MultiByteToWideChar(CP_ACP, 0, fileInfo.name, lenA, unicodeName, lenU);
			unicodeName[lenU] = '\0';

			//verylongfilename.txt -> verylongfile...
			if (lenU > 15) {
				unicodeName[12] = '.';
				unicodeName[13] = '.';
				unicodeName[14] = '.';
				unicodeName[15] = '\0';
			}
			
			sf::Text file(unicodeName, font);
			file.setPosition(sf::Vector2f(0.0, 30.0 * i++));

			files->push_back(file);
		} while (!_findnext(fileHandle, &fileInfo));
	}
	catch (const int errorCode) {
		switch (errorCode) {
		case NO_FILE:
			cout << "getSeatFileList: No File" << endl;
		default:
			break;
		}
	}

	_findclose(fileHandle);

	return files;
}

string FileManager::getSeatFileName(const int index) {
	long fileHandle;
	_finddata_t fileInfo;

	string fileName;

	try {
		if ((fileHandle = _findfirst((programPath + "/seat/*.txt").c_str(), &fileInfo)) == -1) {
			throw(NO_FILE);
		}

		size_t i = 0;
		do {
			if (i == index) {
				fileName = fileInfo.name;
				break;
			}
			else i++;
		} while (!_findnext(fileHandle, &fileInfo));
	}
	catch (const int errorCode) {
		switch (errorCode) {
		case NO_FILE:
			cout << "getSeatFileName: No File" << endl;
		default:
			break;
		}
	}

	_findclose(fileHandle);

	return fileName;
}

byte2d* FileManager::getSeatSquares(const string fileName) {
	FILE* seatFile = fopen((programPath + "/seat/" + fileName).c_str(), "r");

	byte2d* arrangement = new byte2d;

	try {
		if (seatFile == NULL) {
			arrangement->sizeOfRow = 0;
			throw(NO_FILE);
		}
		do {
			//get the information of seat in classroom
			char input[MAX_HORIZONTAL];
			fgets(input, MAX_HORIZONTAL, seatFile);

			size_t len = strlen(input);
			arrangement->sizeOfRow = len;

			vector<byte>& seats = arrangement->arr;

			for (size_t i = 0; i < len; i++) {
				switch (input[i]) {
				case '1':
					seats.push_back(true);
					break;

				case '\0':
				case '\n':
					break;

				default:
					seats.push_back(false);
					break;
				}
			}

		} while (!feof(seatFile));

	}
	catch (const int errorCode) {
		switch (errorCode) {
		case NO_FILE:
			cout << "getSeatSquares: No file" << endl;
			break;

		default:
			break;
		}
	}

	fclose(seatFile);
	return arrangement;
}

vector<wstring>* FileManager::getNames() {
	FILE* nameFile = fopen((programPath + "/names.txt").c_str(), "r");

	vector<wstring>* names = new vector<wstring>;

	try {
		if (nameFile == NULL) {
			names->push_back(L"");
			throw(NO_FILE);
		}
		do {
			char input[20];
			fgets(input, 20, nameFile);

			wchar_t unicodeName[MAX_PATH];

			int lenA = strlen(input);
			int lenU = ::MultiByteToWideChar(CP_ACP, 0, input, lenA, 0, 0);
			::MultiByteToWideChar(CP_ACP, 0, input, lenA, unicodeName, lenU);
			unicodeName[lenU] = '\0';

			names->push_back(unicodeName);
		} while (!feof(nameFile));
	}
	catch (const int errorCode) {
		switch (errorCode) {
		case NO_FILE:
			cout << "getNames: No file" << endl;
			break;

		default:
			break;
		}
	}

	fclose(nameFile);
	return names;
}