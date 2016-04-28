#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "List.hpp"

/**
 * Tests des fonctions et des surcharges de List.hpp
 * Source des tests : http://www.cplusplus.com/reference/list/list/
 * Framework : catch
 **/

 // TODO Test memory leaks

TEST_CASE("Constructors", "[List]") {
	// constructors used in the same order as described above:
	List<int> first;                                // empty list of ints

	REQUIRE(first.empty() == true);

	List<int> second (4,100);                       // four ints with value 100

	REQUIRE(second.size() == 4);
	REQUIRE(second.front() == 100);
	REQUIRE(second.back() == 100);

	List<int> third (second.begin(),second.end());  // iterating through second

	REQUIRE(third.size() == 4);
	REQUIRE(third.front() == 100);
	REQUIRE(third.back() == 100);

	List<int> fourth (third);                       // a copy of third

	REQUIRE(fourth.size() == third.size());
	REQUIRE(fourth.front() == third.front());
	REQUIRE(fourth.back() == third.back());

	// the iterator constructor can also be used to construct from arrays:
	int myints[] = {16,2,77,29};
	List<int> fifth (myints, myints + sizeof(myints) / sizeof(int) );

	REQUIRE(fifth.size() == 4);
	REQUIRE(fifth == List<int>({16,2,77,29}));
}

TEST_CASE("operator=", "[List]") {
	List<int> first (3);      // list of 3 zero-initialized ints
	List<int> second (5);     // list of 5 zero-initialized ints

	second = first;

	REQUIRE(second.size() == 3);

	first = List<int>();

	REQUIRE(first.empty() == true);

	first = List<int>({2, 3});

	REQUIRE(first.size() == 2);
}

TEST_CASE("Iterators", "[List]") {
	List<int> mylist({2, 3, 5});

	SECTION("begin()") {
		List<int>::iterator it = mylist.begin();

		REQUIRE(*it == 2);
	}
	SECTION("end()") {
		List<int>::iterator it = mylist.end();
		it--; // pointe sur 5

		REQUIRE(*it == 5);
	}
	SECTION("rbegin()") {
		List<int>::reverse_iterator it = mylist.rbegin();

		REQUIRE(*it == 5);
	}
	SECTION("rend()") {
		List<int>::reverse_iterator it = mylist.rend();
		it--;

		REQUIRE(*it == 2);
	}
}

TEST_CASE("Capacity", "[List]") {
	List<int> mylist({2, 3, 5});

	SECTION("empty()") {
		REQUIRE(mylist.empty() == false);
	}
	SECTION("size()") {
		REQUIRE(mylist.size() == 3);
	}
}

TEST_CASE("Element access", "[List]") {
	List<int> mylist({2, 100});

	SECTION("front()") {
		REQUIRE(mylist.front() == 2);
	}
	SECTION("back()") {
		REQUIRE(mylist.back() == 100);
	}
}

TEST_CASE("Modifiers", "[List]") {
	SECTION("assign()") {
		List<int> mylist({2, 3, 5});
		List<int> mylist2({10, 20, 30});

		mylist2.assign(mylist.begin(), mylist.end());
		REQUIRE(mylist2 == mylist);

		mylist2.assign(2, 100);
		REQUIRE(mylist2 == List<int>(2, 100));

		mylist2.assign({2, 3, 5});
		REQUIRE(mylist2 == mylist);
	}

	SECTION("emplace_front()") {
		List< std::pair<int,char> > mylist;
		mylist.emplace_front(10,'a');
		mylist.emplace_front(20,'b');
 		mylist.emplace_front(30,'c');
		std::pair<int, char> a(30, 'c');

		REQUIRE(mylist.front() == a);
	}

	SECTION("push_front()") {
		List<int> mylist({100, 200});
		mylist.push_front(300);

		REQUIRE(mylist.front() == 300);
	}

	SECTION("pop_front()") {
		List<int> mylist({100, 200});
		mylist.pop_front();

		REQUIRE(mylist.front() == 200);
	}

	SECTION("emplace_back()") {
		List< std::pair<int,char> > mylist;
		mylist.emplace_back(10,'a');
		mylist.emplace_back(20,'b');
 		mylist.emplace_back(30,'c');
		std::pair<int, char> a(30, 'c');

		REQUIRE(mylist.back() == a);
	}

	SECTION("push_back()") {
		List<int> mylist({100, 200});
		mylist.push_back(300);

		REQUIRE(mylist.back() == 300);
	}

	SECTION("pop_front()") {
		List<int> mylist({100, 200});
		mylist.pop_back();

		REQUIRE(mylist.back() == 100);
	}

	SECTION("emplace_back()") {
		List< std::pair<int,char> > mylist;
		std::pair<int, char> a(200, 'y');

  		mylist.emplace ( mylist.begin(), 100, 'x' );
  		mylist.emplace ( mylist.begin(), 200, 'y' );

		REQUIRE(mylist.front() == a);
	}

	SECTION("insert()") {
		List<int> mylist;
		List<int>::iterator it;

		for (int i=1; i<=5; ++i) mylist.push_back(i); // 1 2 3 4 5

		it = mylist.begin();
		++it;

		mylist.insert (it,10);                        // 1 10 2 3 4 5

		REQUIRE(mylist == List<int>({1,10,2,3,4,5}));

		mylist.insert (it,2,20);                      // 1 10 20 20 2 3 4 5

		REQUIRE(mylist == List<int>({1,10,20,20,2,3,4,5}));

		--it;

		std::vector<int> myvector (2,30);			// 1 10 20 30 30 20 2 3 4 5
		mylist.insert (it,myvector.begin(),myvector.end());

		REQUIRE(mylist == List<int>({1,10,20,30,30,20,2,3,4,5}));
	}

	SECTION("erase()") {
		List<int> mylist;
		List<int>::iterator it1, it2;

		for (int i=1; i<10; ++i) mylist.push_back(i*10);

		                          	// 10 20 30 40 50 60 70 80 90
		it1 = it2 = mylist.begin(); // ^^
		advance (it2,6);            // ^                 ^
		++it1;                      //    ^              ^

		it1 = mylist.erase (it1);   // 10 30 40 50 60 70 80 90
		                          	//    ^           ^

		REQUIRE(mylist == List<int>({10,30,40,50,60,70,80,90}));

		it2 = mylist.erase (it2);   // 10 30 40 50 60 80 90
		                          	//    ^           ^

		REQUIRE(mylist == List<int>({10,30,40,50,60,80,90}));

		++it1;                      //       ^        ^
		--it2;                      //       ^     ^

		mylist.erase (it1,it2);     // 10 30 60 80 90

		REQUIRE(mylist == List<int>({10,30,60,80,90}));
	}

	SECTION("swap()") {
		List<int> first (3,100);   // three ints with a value of 100
  		List<int> second (5,200);  // five ints with a value of 200

  		first.swap(second);

		REQUIRE(first == List<int>(5, 200));
		REQUIRE(second == List<int>(3, 100));
	}

	SECTION("resize()") {
		List<int> mylist;

		for (int i=1; i<10; ++i) mylist.push_back(i);

		mylist.resize(5);
		REQUIRE(mylist.size() == 5);
		REQUIRE(mylist.back() == 5);
		mylist.resize(8,100);
		REQUIRE(mylist.size() == 8);
		REQUIRE(mylist.back() == 100);
		mylist.resize(12);
		REQUIRE(mylist.size() == 12);
		REQUIRE(mylist.back() == 0);
	}

	SECTION("clear()") {
		List<int> mylist({100,200,300});

		mylist.clear();

		REQUIRE(mylist.empty() == true);
	}
}

TEST_CASE("Algorithms", "[List]") {
	SECTION("splice()") {
		/* Version STD */

		std::list<int> list1, list2;
		std::list<int>::iterator it;

		// set some initial values:
		for (int i=1; i<=4; ++i)
			list1.push_back(i);      	// list1: 1 2 3 4
		for (int i=1; i<=3; ++i)
			list2.push_back(i*10);   	// list2: 10 20 30
		it = list1.begin();
		++it;                         	// points to 2
		list1.splice (it, list2); 		// list1: 1 10 20 30 2 3 4
		                            	// list2 (empty)
		                            	// "it" still points to 2 (the 5th element)
		list2.splice (list2.begin(), list1, it);
		                            	// list1: 1 10 20 30 3 4
		                            	// list2: 2
		                            	// "it" is now invalid.
		it = list1.begin();
		std::advance(it,3);           	// "it" points now to 30
		list1.splice ( list1.begin(), list1, it, list1.end());
		                            	// list1: 30 3 4 1 10 20

		/* My list */

		List<int> mylist1, mylist2;
		List<int>::iterator mit;
		// set some initial values:
		for (int i=1; i<=4; ++i)
			mylist1.push_back(i);      	// mylist1: 1 2 3 4

		for (int i=1; i<=3; ++i)
			mylist2.push_back(i*10);   	// mylist2: 10 20 30
		mit = mylist1.begin();
		++mit;                         	// points to 2
		mylist1.splice (mit, mylist2); 	// mylist1: 1 10 20 30 2 3 4
		                            	// mylist2 (empty)
		                            	// "it" still points to 2 (the 5th element)
		mylist2.splice (mylist2.begin(), mylist1, mit);
		                            	// mylist1: 1 10 20 30 3 4
		                            	// mylist2: 2
		                            	// "it" is now invalid.
		mit = mylist1.begin();
		std::advance(mit,3);           	// "it" points now to 30
		mylist1.splice ( mylist1.begin(), mylist1, mit, mylist1.end());
										// list1: 30 3 4 1 10 20

		REQUIRE(mylist1 == list1);
	}

	SECTION("remove()") {
		int myints[]= {17,89,7,14};

		/* Version STD */
  		std::list<int> list (myints,myints+4);
  		list.remove(89);

		/* Ma version */
  		List<int> mylist (myints,myints+4);
  		mylist.remove(89);

		REQUIRE(mylist == list);
	}

	SECTION("remove_if()") {
		// a predicate implemented as a class:
		struct is_odd {
		  bool operator() (const int& value) { return (value%2)==1; }
		};

		int myints[]= {15,36,17,20,39};

  		std::list<int> list (myints,myints+5);   // 15 36 17 20 39
		list.remove_if (is_odd());

		List<int> mylist (myints,myints+5);
		mylist.remove_if (is_odd());

		REQUIRE(mylist == list);
	}

	SECTION("unique()") {
		// a binary predicate implemented as a class:
		struct is_near {
		  bool operator() (double first, double second)
		  { return (fabs(first-second)<5.0); }
		};
		double mydoubles[]={ 12.15,  2.72, 73.0,  12.77,  3.14,
                       12.77, 73.35, 72.25, 15.3,  72.25 };

		std::list<double> list (mydoubles,mydoubles+10);
		list.sort();
		list.unique();
		list.unique(is_near());

		List<double> mylist (mydoubles,mydoubles+10);
		mylist.sort();
		mylist.unique();
		mylist.unique(is_near());

		REQUIRE(mylist == list);
	}

	SECTION("merge()") {
		// compare only integral part:
		struct integral_part {
			bool operator() (double first, double second)
			{ return ( int(first)<int(second) ); }
		};

		std::list<double> first, second;
		first.push_back (3.1);
		first.push_back (2.2);
		first.push_back (2.9);
		second.push_back (3.7);
		second.push_back (7.1);
		second.push_back (1.4);
		first.sort();
		second.sort();
		first.merge(second);
		// (second is now empty)
		second.push_back (2.1);
		first.merge(second,integral_part());

		List<double> myfirst, mysecond;
		myfirst.push_back (3.1);
		myfirst.push_back (2.2);
		myfirst.push_back (2.9);
		mysecond.push_back (3.7);
		mysecond.push_back (7.1);
		mysecond.push_back (1.4);
		myfirst.sort();
		mysecond.sort();
		myfirst.merge(mysecond);
		// (second is now empty)
		mysecond.push_back (2.1);
		myfirst.merge(mysecond,integral_part());

		REQUIRE(myfirst == first);
	}

	SECTION("sort()") {
		// comparison, not case sensitive.
		struct compare_nocase {
			bool operator() (const std::string& first, const std::string& second)
			{
			  unsigned int i=0;
			  while ( (i<first.length()) && (i<second.length()) )
			  {
			    if (tolower(first[i])<tolower(second[i])) return true;
			    else if (tolower(first[i])>tolower(second[i])) return false;
			    ++i;
			  }
			  return ( first.length() < second.length() );
			}
		};

		std::list<std::string> list;
		list.push_back ("one");
		list.push_back ("two");
		list.push_back ("Three");
		list.sort();
		list.sort(compare_nocase());

		List<std::string> mylist;
		mylist.push_back ("one");
		mylist.push_back ("two");
		mylist.push_back ("Three");
		mylist.sort();
		mylist.sort(compare_nocase());

		REQUIRE(mylist == list);
	}

	SECTION("reverse()") {
		std::list<int> list;
		for (int i=1; i<10; ++i) list.push_back(i);
		list.reverse();

		List<int> mylist;
		for (int i=1; i<10; ++i) mylist.push_back(i);
		mylist.reverse();

		REQUIRE(mylist == list);
	}
}

TEST_CASE("Non-member function overloads", "[List]") {
	SECTION("Relationnal operators") {
		List<int> a = {10, 20, 30};
		List<int> b = {10, 20, 30};
		List<int> c = {30, 20, 10};

		REQUIRE(a == b);
		REQUIRE(b != c);
		REQUIRE(b < c);
		REQUIRE(c > b);
		REQUIRE(a <= b);
		REQUIRE(a >= b);
	}

	SECTION("Non-member swap") {
		List<int> foo (3,100);   // three ints with a value of 100
		List<int> bar (5,200);   // five ints with a value of 200

		swap(bar, foo);

		REQUIRE(foo == List<int>(5, 200));
		REQUIRE(bar == List<int>(3, 100));
	}
}
