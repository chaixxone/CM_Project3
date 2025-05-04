#include <catch2/catch_all.hpp>
#include <math.hpp>

TEST_CASE("Normal", "[math]") 
{
	sf::Vector2f a{ 3, 4 };
	sf::Vector2f expected{ -4.0 / 5, 3.0 / 5 };
	sf::Vector2f actual = Engine::Normal(a);
	REQUIRE((actual.x == expected.x && actual.y == expected.y));
}