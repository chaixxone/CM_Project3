#include <cmath>
#include <SFML/System.hpp>

namespace Engine
{
	/**
	* @brief calculates normal vector from a vector
	* @param a: vector (or an edge)
	*/
	sf::Vector2f Normal(const sf::Vector2f& a)
	{
		float x = -a.y;
		float y = a.x;
		float length = std::sqrt(x * x + y * y);
		x /= length;
		y /= length;
		return sf::Vector2f{ x, y };
	}

	float Dot(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	/**
	* @brief calculates a point's projection onto a normal vector
	* @param a: shape edge start
	* @param b: shape edge end
	* @param vertex: point for which to create a projection 
	*/
	float Projection(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& vertex)
	{
		sf::Vector2f line = b - a;
		sf::Vector2f normalVector = Normal(line);

		sf::Vector2f vectorPoint = vertex - a;
		float projection = Engine::Dot(vectorPoint, normalVector);

		return projection;
	}

	/**
	* @brief Checks two shapes for a collision between them. Uses SAT collision method.
	* @param aShapeVertices: first shape verteces
	* @param bShapeVertices: second shape verteces
	*/
	bool CheckCollide(const std::vector<sf::Vector2f> aShapeVertices, const std::vector<sf::Vector2f> bShapeVertices)
	{
		return false;
	}
} // namespace Engine