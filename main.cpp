#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "List.hpp"

/**
 * Insérer les tests après ce commentaire
**/

TEST_CASE("push_back(x) fonctionne", "[List]") {

	List<int> mylist(2);
	mylist.push_back(100);

	REQUIRE(mylist.back() == 100);
}