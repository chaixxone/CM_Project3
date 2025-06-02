#pragma once

#include <cmath>
#include <vector>
#include <SFML/System.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <optional>

#include <projection.hpp>
#include <collision_response.hpp>

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
	float orientedArea(const std::vector<sf::Vector2f>& vertices)
	{
		float sum = 0.f;

		for (size_t i = 0; i < vertices.size(); i++)
		{
			size_t nextIndex = (i + 1) % vertices.size();
			sum += vertices[i].x * vertices[nextIndex].y - vertices[nextIndex].x * vertices[i].y;
		}

		return sum / 2;
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
		float sArea = orientedArea(vertices);

		for (size_t i = 0; i < vertices.size(); i++)
		{
			size_t nextIndex = (i + 1) % vertices.size();
			float ratio = vertices[i].x * vertices[nextIndex].y - vertices[nextIndex].x * vertices[i].y;
			x += (vertices[i].x + vertices[nextIndex].x) * ratio;
			y += (vertices[i].y + vertices[nextIndex].y) * ratio;
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
		float projection = Engine::dot(vertex, normalVector);
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
		float projection = Engine::dot(vertex, normalVector);
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
	* @brief Calculates 2-dimensional cross product of vectors
	* @param a: first vector
	* @param b: second vector
	* @return the direction of vectors rotation
	*/
	float cross(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return a.x * b.y - b.x * a.y;
	}

	struct VectorCollinear
	{
		bool operator()(const sf::Vector2f& left, const sf::Vector2f& right) const
		{
			return cross(left, right) == 0.f;
		}
	};

	struct VectorHash
	{
		std::size_t operator()(const sf::Vector2f& v) const
		{
			float magnitude = std::sqrt(v.x * v.x + v.y * v.y);

			if (magnitude == 0.f)
			{
				return 0;
			}

			float nx = v.x / magnitude;
			float ny = v.y / magnitude;

			int ix = static_cast<int>(nx * 1000);
			int iy = static_cast<int>(ny * 1000);

			return std::hash<int>()(ix) ^ (std::hash<int>()(iy) << 1);
		}
	};

	/**
	* @brief Checks two shapes for a collision between them. Uses SAT collision method and forms collision response
	* @param aShapeVertices: first shape verteces
	* @param bShapeVertices: second shape verteces
	* @return std::nullopt if no collision detected, CollisionResponse in std::optinal when there is collision
	*/
	std::optional<CollisionResponse> processCollision(const std::vector<sf::Vector2f>& aShapeVertices, const std::vector<sf::Vector2f>& bShapeVertices)
	{
		static sf::Vector2f ZERO_VECTOR{ 0.f, 0.f };

		auto aEdges = getShapeEdges(aShapeVertices);
		auto bEdges = getShapeEdges(bShapeVertices);

		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> allEdges = aEdges;
		std::ranges::copy(bEdges, std::back_inserter(allEdges));

		std::vector<Projection> projectionsA(aShapeVertices.size());
		std::vector<Projection> projectionsB(bShapeVertices.size());

		float lengthMTV = std::numeric_limits<float>::infinity();
		sf::Vector2f pointOfCollision;
		sf::Vector2f minimumTranslationVector;		

		for (const auto& edge : allEdges)
		{
			sf::Vector2f edgeVector = edge.second - edge.first;
			sf::Vector2f normalVector = normal(edgeVector);
			float edgeMagnitude = std::sqrt(edgeVector.x * edgeVector.x + edgeVector.y * edgeVector.y);
			sf::Vector2f edgeNormalisedVector = edgeVector / edgeMagnitude;

			// make each vertex projections
			for (size_t j = 0; j < aShapeVertices.size(); j++)
			{
				float projection = projectionWithNormal(edge.first, normalVector, aShapeVertices[j]);
				projectionsA[j] = Projection{ projection, aShapeVertices[j] };
			}

			for (size_t j = 0; j < bShapeVertices.size(); j++)
			{
				float projection = projectionWithNormal(edge.first, normalVector, bShapeVertices[j]);
				projectionsB[j] = Projection{ projection, bShapeVertices[j] };
			}

			// find minimum and maximum projections of each shape
			Projection minProjectionA = projectionsA[0];
			Projection maxProjectionA = projectionsA[0];

			for (size_t j = 1; j < projectionsA.size(); j++)
			{
				minProjectionA = std::min(minProjectionA, projectionsA[j]);
				maxProjectionA = std::max(maxProjectionA, projectionsA[j]);
			}

			Projection minProjectionB = projectionsB[0];
			Projection maxProjectionB = projectionsB[0];

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
					return std::nullopt;
				}

				overlapVectorLength = maxProjectionA - minProjectionB;
				pointOfCollision = maxProjectionA.GetPoint();
			}
			else if (minProjectionA > minProjectionB)
			{
				if (minProjectionA > maxProjectionB)
				{
					return std::nullopt;
				}

				overlapVectorLength = maxProjectionB - minProjectionA;
				pointOfCollision = minProjectionA.GetPoint();
			}

			if (overlapVectorLength < lengthMTV)
			{
				lengthMTV = overlapVectorLength;
				minimumTranslationVector = normalVector * lengthMTV;
			}
		}

		sf::Vector2f Acentroid = centroid(aShapeVertices);
		sf::Vector2f Bcentroid = centroid(bShapeVertices);
		sf::Vector2f directionAB = Acentroid - Bcentroid;

		// if the MTV and the direction from shape A to shape B are opposite, then dot(AB, MTV) < 0
		// which means you have to rotate MTV by pi (negate it)
		if (dot(minimumTranslationVector, directionAB) < 0.f)
		{
			minimumTranslationVector = -minimumTranslationVector;
		}

		CollisionResponse response{ pointOfCollision, minimumTranslationVector };

		return std::optional<CollisionResponse>{ response };
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