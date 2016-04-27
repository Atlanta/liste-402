#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "List.hpp"

/**
 * Ins�rer les tests apr�s ce commentaire
**/

TEST_CASE("push_back(x) fonctionne", "[List]") {

	List<int> mylist(2);
	mylist.push_back(100);

	REQUIRE(mylist.back() == 100);
}