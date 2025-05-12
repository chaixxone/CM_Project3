#include <cmath>
#include <SFML/System.hpp>

namespace Engine
{
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
} // namespace Engine