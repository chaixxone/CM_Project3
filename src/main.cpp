#include <vector>

#include <SFML/Graphics.hpp>

#include <math.hpp>

int main()
{
	sf::RenderWindow window{ sf::VideoMode{800, 600}, "window" };

	sf::RectangleShape rect1({ 200, 100 });
	sf::RectangleShape rect2({ 200, 100 });

	rect1.setPosition({ 200, 250 });
	rect1.setRotation(-10);
	rect2.setPosition({ 410, 230 });
	rect2.setRotation(-12);

	constexpr float POINT_RADIUS = 10.0f;
	// a red point that align to the origin of the first rectangle
	sf::CircleShape rect1Point{ POINT_RADIUS };
	rect1Point.setFillColor(sf::Color::Red);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			// configure movement
			if (event.type == sf::Event::KeyPressed)
			{
				constexpr float VELOCITY = 10.0f;

				sf::Keyboard::Key key = event.key.code;

				switch (key)
				{
				case sf::Keyboard::W:
					rect1.move(sf::Vector2f{ 0.f, -VELOCITY });
					break;
				case sf::Keyboard::A:
					rect1.move(sf::Vector2f{ -VELOCITY, 0.f });
					break;
				case sf::Keyboard::S:
					rect1.move(sf::Vector2f{ 0.f, VELOCITY });
					break;
				case sf::Keyboard::D:
					rect1.move(sf::Vector2f{ VELOCITY, 0.f });
					break;
				case sf::Keyboard::Up:
					rect2.move(sf::Vector2f{ 0.f, -VELOCITY });
					break;
				case sf::Keyboard::Left:
					rect2.move(sf::Vector2f{ -VELOCITY, 0.f });
					break;
				case sf::Keyboard::Down:
					rect2.move(sf::Vector2f{ 0.f, VELOCITY });
					break;
				case sf::Keyboard::Right:
					rect2.move(sf::Vector2f{ VELOCITY, 0.f });
					break;
				default:
					break;
				}
			}

			if (event.type == sf::Event::MouseWheelScrolled)
			{
				float scrollDirection = event.mouseWheelScroll.delta;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
				{
					rect2.rotate(scrollDirection);
				}
				else
				{
					rect1.rotate(scrollDirection);
				}
			}

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear(sf::Color::Black);

		// move a red point to the origin of the first shape
		sf::Vector2f pos1 = rect1.getPosition();
		rect1Point.setPosition(sf::Vector2f{ pos1.x - POINT_RADIUS, pos1.y - POINT_RADIUS });

		auto verticesRect1 = Engine::getVertices(&rect1);
		auto verticesRect2 = Engine::getVertices(&rect2);

		// paint the rectangles red if they are colliding
		if (Engine::checkCollide(verticesRect1, verticesRect2))
		{
			rect1.setFillColor(sf::Color::Red);
			rect2.setFillColor(sf::Color::Red);
		}

		window.draw(rect1);
		window.draw(rect2);
		window.draw(rect1Point);

		// paint the rectangles white
		rect1.setFillColor(sf::Color::White);
		rect2.setFillColor(sf::Color::White);

		window.display();
	}

	return 0;
}