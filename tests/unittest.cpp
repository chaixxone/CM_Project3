#include <catch2/catch_all.hpp>
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
	sf::Vector2f line{ fillerPoint.x - origin.x, fillerPoint.y - origin.y };
	sf::Vector2f normal = Engine::Normal(line);

	sf::Vector2f point{ 7.f, 7.f };
	sf::Vector2f vectorPoint{ point.x - origin.x, point.y - origin.y };
	float distanceNormal = Engine::Dot(vectorPoint, normal);

	sf::Vector2f productDistanceNormal = normal * distanceNormal;
	sf::Vector2f projectionPoint{ point.x - productDistanceNormal.x , point.y - productDistanceNormal.y };
	INFO("projectionPoint = (" << projectionPoint.x << ", " << projectionPoint.y << ")");

	sf::Vector2f expected{ 7.38f, 5.715f };

	float tolerance = 1e-2;
	// assert projection point coordinates is near expected variable coordinates
	REQUIRE((abs(expected.x - projectionPoint.x) < tolerance && abs(expected.y - projectionPoint.y) < tolerance));
}