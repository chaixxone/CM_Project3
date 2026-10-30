#pragma once

#include <abstraction.hpp>
#include <SFML/graphics.hpp>

namespace Implementation
{
	class Rectangle : public Engine::IShape
	{
	public:
		Rectangle(float width, float height);
		~Rectangle();

		size_t GetPointCount() override;

		std::vector<Engine::Vector2f> GetVertices() override;

		void Move(Engine::Vector2f& direction) override;

		void SetPosition(const Engine::Vector2f& position) override;

	private:
		sf::Shape* m_shape;
	};
} // namespace Implementation