#include <catch2/catch_all.hpp>
#include <catch2/catch_approx.hpp>
#include <math.hpp>

TEST_CASE("Normal", "[math]")
{
	sf::Vector2f a{ 3, 4 };
	sf::Vector2f expected{ -4.0 / 5, 3.0 / 5 };
	sf::Vector2f actual = Engine::Normal(a);
	REQUIRE((actual.x == expected.x && actual.y == expected.y));
}

TEST_CASE("Projection", "[math]")
{
	sf::Vector2f origin{ 5.f, 5.f };
	sf::Vector2f fillerPoint{ 6.f, 5.3f };
	sf::Vector2f line = fillerPoint - origin;
	sf::Vector2f normal = Engine::Normal(line);

	sf::Vector2f point{ 7.f, 7.f };
	sf::Vector2f vectorPoint = point - origin;
	float distanceNormal = Engine::Dot(vectorPoint, normal);

	sf::Vector2f productDistanceNormal = normal * distanceNormal;
	sf::Vector2f projectionPoint = point - productDistanceNormal;
	INFO("projectionPoint = (" << projectionPoint.x << ", " << projectionPoint.y << ")");

	sf::Vector2f expected{ 7.38f, 5.715f };

	float tolerance = 1e-2;
	// make sure that the 2D coordinates of projected point are close to the expected coordinates
	REQUIRE(Catch::Approx(projectionPoint.x).margin(tolerance) == expected.x);
	REQUIRE(Catch::Approx(projectionPoint.y).margin(tolerance) == expected.y);
}