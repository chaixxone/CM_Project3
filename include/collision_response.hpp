#include <SFML/System/Vector2.hpp>

namespace Engine
{
	struct CollisionResponse
	{
		CollisionResponse(const sf::Vector2f& point, const sf::Vector2f& MTV)
			: PointOfCollision(point), MinimumTransitionVector(MTV) {}

		sf::Vector2f PointOfCollision;
		sf::Vector2f MinimumTransitionVector;
	};
} // namespace Engine