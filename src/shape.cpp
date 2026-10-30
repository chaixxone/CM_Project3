#include <shape.hpp>
#include <math.hpp>

namespace Implementation
{
	Rectangle::Rectangle(float width, float height)
	{
		m_shape = new sf::RectangleShape({ width, height });
	}

	Rectangle::~Rectangle()
	{
		delete m_shape;
	}

	size_t Rectangle::GetPointCount() { return 4; }

	std::vector<Engine::Vector2f> Rectangle::GetVertices()
	{
		std::vector<sf::Vector2f> vertices = Engine::getVertices(m_shape);
		std::vector<Engine::Vector2f> convertedVertises;
		convertedVertises.reserve(vertices.size());

		for (size_t i = 0; i < vertices.size(); i++)
		{
			convertedVertises[i].x = vertices[i].x;
			convertedVertises[i].y = vertices[i].y;
		}

		return convertedVertises;
	}

	void Rectangle::Move(Engine::Vector2f& direction)
	{
		m_shape->move({ direction.x, direction.y });
	}

	void Rectangle::Move(Engine::Vector2f&& direction)
	{
		m_shape->move({ direction.x, direction.y });
	}

	void Rectangle::SetPosition(const Engine::Vector2f& position)
	{
		m_shape->setPosition({ position.x, position.y });
	}
} // namespace Implementation