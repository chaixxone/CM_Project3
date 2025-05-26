#pragma once

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
	* @brief calculates area with given shape vertices
	* @param vertices: shape vertices
	* @returns shape area
	*/
	float area(const std::vector<sf::Vector2f>& vertices)
	{
		float sum = 0.f;

		for (size_t i = 0; i < vertices.size() - 1; i++)
		{
			sum += vertices[i].x * vertices[i + 1].y - vertices[i + 1].x * vertices[i].y;
		}

		return std::abs(sum / 2);
	}

	/**
	* @brief calculates centre point (centroid) of the shape by given vertices
	* @param vertices: shape vertices
	* @returns coordinates of centroid
	*/
	sf::Vector2f centroid(const std::vector<sf::Vector2f>& vertices)
	{
		float x = 0.f;
		float y = 0.f;
		float sArea = area(vertices);

		for (size_t i = 0; i < vertices.size() - 1; i++)
		{
			x += (vertices[i].x * vertices[i + 1].x) * (vertices[i].x * vertices[i + 1].y - vertices[i + 1].x * vertices[i].y);
			y += (vertices[i].y * vertices[i + 1].y) * (vertices[i].x * vertices[i + 1].y - vertices[i + 1].x * vertices[i].y);
		}

		return sf::Vector2f{ x, y } / (6 * sArea);
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
	* @brief calculates a point's projection onto a normal vector
	* @param start: shape edge start
	* @param normalVector: edge's normal
	* @param vertex: point for which to create a projection
	* @returns point 1D projection onto a normal's axis
	*/
	float projectionWithNormal(const sf::Vector2f& start, const sf::Vector2f& normalVector, const sf::Vector2f& vertex)
	{
		sf::Vector2f vectorPoint = vertex - start;
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
	* @return zero vector if no collision detected, MTV if shapes are colliding
	*/
	sf::Vector2f checkCollide(const std::vector<sf::Vector2f>& aShapeVertices, const std::vector<sf::Vector2f>& bShapeVertices)
	{
		auto aEdges = getShapeEdges(aShapeVertices);
		auto bEdges = getShapeEdges(bShapeVertices);

		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> allEdges = aEdges;
		std::ranges::copy(bEdges, std::back_inserter(allEdges));

		std::vector<float> projectionsA(aShapeVertices.size());
		std::vector<float> projectionsB(bShapeVertices.size());

		float lengthMTV = std::numeric_limits<float>::infinity();
		sf::Vector2f minimumTranslationVector;

		for (const auto& edge : allEdges)
		{
			sf::Vector2f normalVector = normal(edge.second - edge.first);

			// make each vertex projections
			for (size_t j = 0; j < aShapeVertices.size(); j++)
			{
				projectionsA[j] = projectionWithNormal(edge.first, normalVector, aShapeVertices[j]);
			}

			for (size_t j = 0; j < bShapeVertices.size(); j++)
			{
				projectionsB[j] = projectionWithNormal(edge.first, normalVector, bShapeVertices[j]);
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

			float overlapVectorLength = 0.f;
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
					return sf::Vector2f{ 0.f, 0.f };
				}

				overlapVectorLength = maxProjectionA - minProjectionB;
			}
			else if (minProjectionA > minProjectionB)
			{
				if (minProjectionA > maxProjectionB)
				{
					return sf::Vector2f{ 0.f, 0.f };
				}

				overlapVectorLength = maxProjectionB - minProjectionA;
			}

			if (overlapVectorLength < lengthMTV)
			{
				lengthMTV = overlapVectorLength;
				minimumTranslationVector = normalVector * lengthMTV;
			}
		}

		return minimumTranslationVector;
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

	/**
	* @brief Calculates 2-dimensional cross product of vectors
	* @param a: first vector 
	* @param b: second vector
	* @return the direction of vectors rotation
	*/
	float cross(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return a.x * b.y - b.x * a.y;
	}

	/**
	* @brief Checks if the shape is concave
	* @param shape: the inspected shape
	* @returns true if the shape is concave, false if it's not
	*/
	bool isShapeConcave(sf::Shape* shape)
	{
		float previousRotateDirection = 0.f;
		size_t verticesCount = shape->getPointCount();
		std::vector<sf::Vector2f> vertices = getVertices(shape);

		for (size_t i = 0; i < verticesCount; i++)
		{
			sf::Vector2f vectorPreviousVertex{ vertices[(i - 1 + verticesCount) % verticesCount] - vertices[i] };
			sf::Vector2f vectorNextVertex{ vertices[(i + 1 + verticesCount) % verticesCount] - vertices[i] };
			float currentRotateDirection = cross(vectorPreviousVertex, vectorNextVertex);

			if (currentRotateDirection * previousRotateDirection < 0)
			{
				return true;
			}

			previousRotateDirection = currentRotateDirection;
		}

		return false;
	}
} // namespace Engine