#include <catch2/catch_all.hpp>
#include <catch2/catch_approx.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <math.hpp>

TEST_CASE("Normal", "[math]")
{
	sf::Vector2f a{ 3.f, 4.f };
	sf::Vector2f expected{ -4.f / 5.f, 3.f / 5.f };
	sf::Vector2f actual = Engine::normal(a);
	REQUIRE(actual == expected);
}

TEST_CASE("Projection", "[math]")
{
	sf::Vector2f origin{ 5.f, 5.f };
	sf::Vector2f fillerPoint{ 6.f, 5.3f };
	sf::Vector2f point{ 7.f, 7.f };

	float distanceNormal = Engine::projection(origin, fillerPoint, point);
	sf::Vector2f normal = Engine::normal(fillerPoint - origin);
	float distanceNormal2 = Engine::projectionWithNormal(normal, point);
	
	float expected = 4.69f;
	float tolerance = 1e-2f;
	// make sure that the 2D coordinates of projected point are close to the expected coordinates
	REQUIRE(distanceNormal == distanceNormal2);
	REQUIRE(Catch::Approx(distanceNormal).margin(tolerance) == expected);
}

TEST_CASE("Projection to 2D Point", "[math]")
{
	const sf::Vector2f expected{ 8.f, 8.f };

	sf::Vector2f A{ 5.f, 5.f };
	sf::Vector2f B{ 6.f, 5.3f };

	sf::Vector2f AB = B - A;
	sf::Vector2f normalisedAB = AB / std::sqrt(AB.x * AB.x + AB.y * AB.y);
	sf::Vector2f normal = Engine::normal(AB);

	const float projectionAB = Engine::dot(expected, normalisedAB);
	const float projectionNormal = Engine::dot(expected, normal);

	sf::Vector2f calculatedPoint = projectionAB * normalisedAB + projectionNormal * normal;
	INFO("calculated point: " << calculatedPoint.x << ' ' << calculatedPoint.y);

	float tolerance = 1e-2f;
	REQUIRE(Catch::Approx(calculatedPoint.x).margin(tolerance) == expected.x);
	REQUIRE(Catch::Approx(calculatedPoint.y).margin(tolerance) == expected.y);
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

	auto noCollision = Engine::processCollision(verticesA, verticesB);
	REQUIRE(noCollision == std::nullopt);
	
	// shift shapeB by 100 pixels to the left - collision must be detected
	shapeB.move({ -100.f, 0.f });
	verticesA = Engine::getVertices(&shapeA);
	verticesB = Engine::getVertices(&shapeB);

	// react to collision
	auto collisionResponse = Engine::processCollision(verticesA, verticesB);
	sf::Vector2f MTV = collisionResponse->MinimumTransitionVector;
	shapeB.move(-MTV);

	// check if the shapeB is moved and doesn't collide anymore
	verticesA = Engine::getVertices(&shapeA);
	verticesB = Engine::getVertices(&shapeB);
	
	// MTV can be sf::Vector{ -0, -0 } which means response is not nullopt
	auto collisionResponseAfterMove = Engine::processCollision(verticesA, verticesB); 
	sf::Vector2f MTV_AfterMove = collisionResponseAfterMove->MinimumTransitionVector;
	float MTV_Magnitude = std::sqrt(MTV_AfterMove.x * MTV_AfterMove.x + MTV_AfterMove.y * MTV_AfterMove.y);

	const float MARGIN = 1e-2f;

	REQUIRE(collisionResponseAfterMove != std::nullopt);
	REQUIRE(Catch::Approx(MTV_Magnitude).margin(MARGIN) == 0.f);
}


TEST_CASE("is shape is concave or is convex", "[math]")
{
	const int PENTAGON = 5;
	const int RECT = 4;
	sf::ConvexShape rect{ RECT };
	rect.setPoint(0, { 0.f, 0.f });
	rect.setPoint(1, { 0.f, 1.f });
	rect.setPoint(2, { 1.f, 1.f });
	rect.setPoint(3, { 1.f, 0.f });
	REQUIRE_FALSE(Engine::isShapeConcave(&rect)); // convex

	sf::ConvexShape pentagon{ PENTAGON };
	pentagon.setPoint(0, { 0.f, 0.f });
	pentagon.setPoint(1, { 0.f, 1.f });
	pentagon.setPoint(2, { 0.5f, 0.5f });
	pentagon.setPoint(3, { 1.f, 1.f });
	pentagon.setPoint(4, { 1.f, 0.f });
	REQUIRE(Engine::isShapeConcave(&pentagon)); // concave

	pentagon.setPoint(2, { 0.5f, 1.5f });
	REQUIRE_FALSE(Engine::isShapeConcave(&pentagon)); // convex
}

TEST_CASE("shapes areas", "[math]")
{
	sf::ConvexShape egyptianTriangle{ 3 };
	egyptianTriangle.setPoint(0, { 0.f, 0.f });
	egyptianTriangle.setPoint(1, { 0.f, 3.f });
	egyptianTriangle.setPoint(2, { 4.f, 3.f });
	float triangleArea = std::abs(Engine::orientedArea(Engine::getVertices(&egyptianTriangle)));
	float expectedTriangleArea = 6.f;
	REQUIRE(triangleArea == expectedTriangleArea);

	const float SQUARE_SIDE = 4.f;
	sf::RectangleShape square{ { SQUARE_SIDE, SQUARE_SIDE } };
	float squareArea = std::abs(Engine::orientedArea(Engine::getVertices(&square)));
	float expectedSquareArea = SQUARE_SIDE * SQUARE_SIDE;
	REQUIRE(squareArea == expectedSquareArea);
}

TEST_CASE("shape centroid", "[math]")
{
	sf::RectangleShape square{ {4.f, 4.f } };
	sf::Vector2f centroid = Engine::centroid(Engine::getVertices(&square));
	INFO("Square centroid: (" << centroid.x << "; " << centroid.y << ")");
	REQUIRE(centroid == sf::Vector2{ 2.f, 2.f });
	
	square.move({ 3.f, 0.f });
	sf::Vector2f movedExpected{ 5.f, 2.f };
	centroid = Engine::centroid(Engine::getVertices(&square));
	INFO("Square centroid: (" << centroid.x << "; " << centroid.y << ")");
	REQUIRE(movedExpected == centroid);
}