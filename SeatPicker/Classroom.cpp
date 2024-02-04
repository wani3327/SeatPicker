#include "Classroom.h"

Classroom::Classroom() { }

void Classroom::setLayout(const byte2d* layout) {
	for (int i = 0; i < layout->arr.size(); i++) {
		if (layout->arr[i] == 1) {
			emptySeats.push_back({ i % layout->sizeOfRow, i / layout->sizeOfRow });
		}
	}

}

list<Seat>* Classroom::randomize() {
	list<Seat>* randomized = new list<Seat>;

	while (!emptySeats.empty()) {
		size_t index = rand() % emptySeats.size();
		list<Seat>::iterator iter = emptySeats.begin();

		for (size_t i = 0; i < index; i++) iter++;

		randomized->push_back(*iter);
		emptySeats.erase(iter);
	}

	return randomized;
}