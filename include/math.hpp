#include <cmath>
#include <vector>
#include <SFML/System.hpp>
#include <SFML/Graphics/Shape.hpp>

namespace Engine
{
	/**
	* @brief calculates normal vector from a vector
	* @param a: vector (or an edge)
	*/
	sf::Vector2f normal(const sf::Vector2f& a)
	{
		float x = -a.y;
		float y = a.x;
		float length = std::sqrt(x * x + y * y);
		x /= length;
		y /= length;
		return sf::Vector2f{ x, y };
	}

	float dot(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	/**
	* @brief calculates a point's projection onto a normal vector
	* @param a: shape edge start
	* @param b: shape edge end
	* @param vertex: point for which to create a projection 
	*/
	float projection(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& vertex)
	{
		sf::Vector2f line = b - a;
		sf::Vector2f normalVector = normal(line);

		sf::Vector2f vectorPoint = vertex - a;
		float projection = Engine::dot(vectorPoint, normalVector);

		return projection;
	}

	/**
	* @brief calculates shape's edges as sf::Vector2f vectors
	* @param shapeVertices: shape vertices coordinates
	*/
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> getShapeEdges(const std::vector<sf::Vector2f>& shapeVertices)
	{
		const size_t VERTICES = shapeVertices.size();
		const size_t LAST = VERTICES - 1;
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> edges(VERTICES);

		for (size_t i = 1; i < VERTICES; i++)
		{
			edges[i - 1] = std::make_pair(shapeVertices[i - 1], shapeVertices[i]);
		}

		edges[LAST] = std::make_pair(shapeVertices[LAST], shapeVertices[0]);

		return edges;
	}	

	/**
	* @brief Checks two shapes for a collision between them. Uses SAT collision method.
	* @param aShapeVertices: first shape verteces
	* @param bShapeVertices: second shape verteces
	* @return false if no collision detected, true if shapes are colliding
	*/
	bool checkCollide(const std::vector<sf::Vector2f>& aShapeVertices, const std::vector<sf::Vector2f>& bShapeVertices)
	{
		auto aEdges = getShapeEdges(aShapeVertices);
		auto bEdges = getShapeEdges(bShapeVertices);

		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> allEdges = aEdges;
		std::ranges::copy(bEdges, std::back_inserter(allEdges));

		std::vector<float> projectionsA(aShapeVertices.size());
		std::vector<float> projectionsB(bShapeVertices.size());

		for (const auto& edge : allEdges)
		{
			// make each vertex projections
			for (size_t j = 0; j < aShapeVertices.size(); j++)
			{
				projectionsA[j] = projection(edge.first, edge.second, aShapeVertices[j]);
			}

			for (size_t j = 0; j < bShapeVertices.size(); j++)
			{
				projectionsB[j] = projection(edge.first, edge.second, bShapeVertices[j]);
			}

			// find minimum and maximum projections of each shape
			float minProjectionA = projectionsA[0];
			float maxProjectionA = projectionsA[0];

			for (size_t j = 1; j < projectionsA.size(); j++)
			{
				minProjectionA = std::min(minProjectionA, projectionsA[j]);
				maxProjectionA = std::max(maxProjectionA, projectionsA[j]);
			}

			float minProjectionB = projectionsB[0];
			float maxProjectionB = projectionsB[0];

			for (size_t j = 1; j < projectionsB.size(); j++)
			{
				minProjectionB = std::min(minProjectionB, projectionsB[j]);
				maxProjectionB = std::max(maxProjectionB, projectionsB[j]);
			}

			/*
			* collision checking rules
			* 
			* for Amin < Bmin
			* Amin--------Bmin=====Amax--------Bmax
			* 			      
			* for Amin > Bmin
			* Bmin--------Amin=====Bmax--------Amax
			*/
			if (minProjectionA < minProjectionB)
			{
				if (minProjectionB > maxProjectionA)
				{
					return false;
				}
			}
			else if (minProjectionA > minProjectionB)
			{
				if (minProjectionA > maxProjectionB)
				{
					return false;
				}
			}
		}

		return true;
	}

	/**
	* @brief Gets all vertex positions from a shape
	* @param shape: a pointer to shape
	* @returns a vector with actual vertex coordinates
	*/
	std::vector<sf::Vector2f> getVertices(sf::Shape* shape)
	{
		size_t verticesAmount = shape->getPointCount();
		std::vector<sf::Vector2f> verteces(verticesAmount);
		sf::Transform transform = shape->getTransform();

		for (size_t i = 0; i < verticesAmount; i++)
		{
			verteces[i] = transform.transformPoint(shape->getPoint(i));
		}

		return verteces;
	}
} // namespace Engine