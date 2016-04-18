#include "List.hpp"
#include <iostream>
#include <vector>

#ifdef _DEBUG
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

using namespace std;

int main(void) {
	List<int> foo(3, 100);   // three ints with a value of 100
	List<int> bar(5, 200);   // five ints with a value of 200

	bar.swap(foo);

	std::cout << "foo contains:";
	for (List<int>::iterator it = foo.begin(); it != foo.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

	std::cout << "bar contains:";
	for (List<int>::iterator it = bar.begin(); it != bar.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

	DEBUG(system("pause");)
	return 0;
}
