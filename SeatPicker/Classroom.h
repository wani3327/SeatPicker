#ifndef CLASSROOM_H
#define CLASSROOM_H

#include <SFML/Graphics.hpp>
#include <list>
#include "FileManager.h"

using namespace std;

struct Seat {
	unsigned int x;
	unsigned int y;
};

class Classroom {
private:
	list<Seat> emptySeats;

public:
	Classroom();
	void setLayout(const byte2d*);
	list<Seat>* randomize();
};


#endif // !CLASSROOM_H