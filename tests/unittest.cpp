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

TEST_CASE("Shape's vertices", "[math]")
{
	const int PENTAGON = 5;
	sf::ConvexShape convexShape{ PENTAGON };
	std::vector<sf::Vector2f> actualVertices = {
		{ 50.f,    2.f    },
		{ 89.f,    10.5f  },
		{ 92.f,    177.1f },
		{ 50.f,    90.f   },
		{ 66.666f, 71.89f }
	};

	for (int i = 0; i < PENTAGON; i++)
	{
		convexShape.setPoint(i, actualVertices[i]);
	}

	std::vector<sf::Vector2f> receivedVertices = Engine::getVertices(&convexShape);

	for (int i = 0; i < PENTAGON; i++)
	{
		REQUIRE(receivedVertices[i] == actualVertices[i]);
	}
}

TEST_CASE("Shape's edges", "[math]")
{
	const int EDGES = 4;
	sf::ConvexShape convexShape{ EDGES };
	std::vector<sf::Vector2f> vertices = {
		{ 50.f, 2.f    },
		{ 89.f, 10.5f  },
		{ 92.f, 177.1f },
		{ 50.f, 90.f   }
	};

	for (int i = 0; i < EDGES; i++)
	{
		convexShape.setPoint(i, vertices[i]);
	}

	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> actualEdges = {
		{ vertices[0], vertices[1] },
		{ vertices[1], vertices[2] },
		{ vertices[2], vertices[3] },
		{ vertices[3], vertices[0] }
	};

	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> receivedEdges = Engine::getShapeEdges(vertices);

	const float MARGIN = 1e-2f;

	for (int i = 0; i < EDGES; i++)
	{
		REQUIRE(Catch::Approx(receivedEdges[i].first.x).margin(MARGIN) == actualEdges[i].first.x);
		REQUIRE(Catch::Approx(receivedEdges[i].first.y).margin(MARGIN) == actualEdges[i].first.y);
		
		REQUIRE(Catch::Approx(receivedEdges[i].second.x).margin(MARGIN) == actualEdges[i].second.x);
		REQUIRE(Catch::Approx(receivedEdges[i].second.y).margin(MARGIN) == actualEdges[i].second.y);
	}
}

TEST_CASE("SAT", "[math]")
{
	const int EDGES = 4;
	sf::ConvexShape shapeA{ EDGES };
	shapeA.setPoint(0, { 100, 400 });
	shapeA.setPoint(1, { 300, 300 });
	shapeA.setPoint(2, { 300, 200 });
	shapeA.setPoint(3, { 100, 100 });

	sf::ConvexShape shapeB{ EDGES };
	shapeB.setPoint(0, { 300, 400 });
	shapeB.setPoint(1, { 500, 500 });
	shapeB.setPoint(2, { 600, 200 });
	shapeB.setPoint(3, { 400, 200 });

	auto verticesA = Engine::getVertices(&shapeA);
	auto verticesB = Engine::getVertices(&shapeB);

	REQUIRE(Engine::checkCollide(verticesA, verticesB) == false);
	
	// shift shapeB by 100 pixels to the left - collission must be detected
	shapeB.move({ -100.f, 0.f });
	verticesA = Engine::getVertices(&shapeA);
	verticesB = Engine::getVertices(&shapeB);

	REQUIRE(Engine::checkCollide(verticesA, verticesB) == true);
}