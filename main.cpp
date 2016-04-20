#include "List.h"
#include <iostream>
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
	List<int> third(second.begin(), second.end());  // iterating through second
	List<int> fourth(third);                       // a copy of third

														// the iterator constructor can also be used to construct from arrays:
	int myints[] = { 16,2,77,29 };
	List<int> fifth(myints, myints + sizeof(myints) / sizeof(int));

	std::cout << "The contents of fifth are: ";
	for (List<int>::iterator it = fifth.begin(); it != fifth.end(); it++)
		std::cout << *it << ' ';

	std::cout << '\n';

	DEBUG(system("pause");)
	return 0;
}
