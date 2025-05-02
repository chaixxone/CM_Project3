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
} // namespace Engine