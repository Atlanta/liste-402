#ifndef COUNTER
#define COUNTER

#include <iostream>

#ifdef _DEBUG
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

using namespace std;

static int construct = 0;
static int copy_construct = 0;
static int move_construct = 0;
static int copy_assignation = 0;
static int move_assignation = 0;
static int destruct = 0;

class Counter {
public:
	Counter () {
		construct++;
		DEBUG(cout << "Counter : Construction" << endl;)
	};
	Counter (const Counter& other) {
		copy_construct++;
		DEBUG(cout << "Counter : Construction par copie" <<  endl;)
	};
	Counter (Counter&& c) {
		move_construct++;
		DEBUG(cout << "Counter : Construction par déplacement" <<  endl;)
	};
	~Counter () {
		destruct++;
		DEBUG(cout << "Counter : Destruction" << endl;)
	}
	Counter& operator=(const Counter& other) {
		if (&other != this) {
			copy_assignation++;
		}
		return *this;
	}
	Counter& operator=(Counter&& other) {
		if (&other != this)
			move_assignation++;
		return *this;
	}
	friend ostream& operator<<(ostream& stream, const Counter& counter) {
		stream << "Constructions : " << construct << endl;
		stream << "Copies : " << copy_construct << endl;
		stream << "Destructions : " << destruct << endl;
		return stream;
	}
};

#endif /* end of include guard: COUNTER */
