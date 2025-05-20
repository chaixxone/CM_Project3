#include <catch2/catch_all.hpp>
#include <catch2/catch_approx.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <math.hpp>

TEST_CASE("Normal", "[math]")
{
	sf::Vector2f a{ 3.f, 4.f };
	sf::Vector2f expected{ -4.f / 5.f, 3.f / 5.f };
	sf::Vector2f actual = Engine::normal(a);
	REQUIRE((actual.x == expected.x && actual.y == expected.y));
}

TEST_CASE("Projection", "[math]")
{
	sf::Vector2f origin{ 5.f, 5.f };
	sf::Vector2f fillerPoint{ 6.f, 5.3f };
	sf::Vector2f point{ 7.f, 7.f };

	sf::Vector2f normalVector = Engine::normal(fillerPoint - origin);
	float distanceNormal = Engine::projection(origin, fillerPoint, point);

	sf::Vector2f productDistanceNormal = normalVector * distanceNormal;
	sf::Vector2f projectionPoint = point - productDistanceNormal;
	INFO("projectionPoint = (" << projectionPoint.x << ", " << projectionPoint.y << ")");

	sf::Vector2f expected{ 7.38f, 5.715f };

	float tolerance = 1e-2f;
	// make sure that the 2D coordinates of projected point are close to the expected coordinates
	REQUIRE(Catch::Approx(projectionPoint.x).margin(tolerance) == expected.x);
	REQUIRE(Catch::Approx(projectionPoint.y).margin(tolerance) == expected.y);
}

TEST_CASE("Shape's verteces", "[math]")
{
	const int PENTAGON = 5;
	sf::ConvexShape convexShape{ PENTAGON };
	std::vector<sf::Vector2f> actualVerteces = {
		{ 50.f,    2.f    },
		{ 89.f,    10.5f  },
		{ 92.f,    177.1f },
		{ 50.f,    90.f   },
		{ 66.666f, 71.89f }
	};

	for (int i = 0; i < PENTAGON; i++)
	{
		convexShape.setPoint(i, actualVerteces[i]);
	}

	std::vector<sf::Vector2f> receivedVerteces = Engine::getVertices(&convexShape);

	for (int i = 0; i < PENTAGON; i++)
	{
		REQUIRE(receivedVerteces[i] == actualVerteces[i]);
	}
}

TEST_CASE("Shape's edges", "[math]")
{
	const int EDGES = 4;
	sf::ConvexShape convexShape{ EDGES };
	std::vector<sf::Vector2f> verteces = {
		{ 50.f,    2.f    },
		{ 89.f,    10.5f  },
		{ 92.f,    177.1f },
		{ 50.f,    90.f   }
	};

	for (int i = 0; i < EDGES; i++)
	{
		convexShape.setPoint(i, verteces[i]);
	}

	std::vector<sf::Vector2f> actualEdges = {
		{ 39.f,  8.5f   },
		{ 3.f,   166.6f },
		{-42.f, -87.1f  },
		{ 0.f,   88.f   }
	};

	std::vector<sf::Vector2f> receivedEdges = Engine::getShapeEdges(verteces);

	const float MARGIN = 1e-2;

	for (int i = 0; i < EDGES; i++)
	{
		REQUIRE(Catch::Approx(receivedEdges[i].x).margin(MARGIN) == actualEdges[i].x);
		REQUIRE(Catch::Approx(receivedEdges[i].y).margin(MARGIN) == actualEdges[i].y);
	}
}