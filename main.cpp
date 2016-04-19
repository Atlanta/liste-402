#include <iostream>
#include "List.h"
#include <vector>

#ifdef _DEBUG
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

using namespace std;

int main(void) {
	// constructors used in the same order as described above:
	List<int> first;                                // empty list of ints
	List<int> second(4, 100);                       // four ints with value 100
	

	DEBUG(system("pause");)
	return 0;
}