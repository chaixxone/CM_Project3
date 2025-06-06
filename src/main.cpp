#include <vector>

#include <SFML/Graphics.hpp>

#include <math.hpp>

int main()
{
	sf::RenderWindow window{ sf::VideoMode{800, 600}, "window" };

	sf::RectangleShape obj({ 200, 100 });
	
	sf::RectangleShape movableMapRect({ 200, 100 });
	sf::RectangleShape staticMapRect({ 200, 300 });

	std::vector<sf::Shape*> map{ &movableMapRect, &staticMapRect };

	obj.setPosition({ 200, 250 });
	obj.setRotation(-10);
	
	movableMapRect.setPosition({ 410, 230 });
	movableMapRect.setRotation(-12);		
	staticMapRect.setPosition({ 600, 300 });

	const float g = 0.00005f;
	float currentObjFallVelocity = 0.f;

	while (window.isOpen())
	{
		currentObjFallVelocity += g;

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
					obj.move(sf::Vector2f{ 0.f, -VELOCITY });
					break;
				case sf::Keyboard::A:
					obj.move(sf::Vector2f{ -VELOCITY, 0.f });
					break;
				case sf::Keyboard::S:
					obj.move(sf::Vector2f{ 0.f, VELOCITY });
					break;
				case sf::Keyboard::D:
					obj.move(sf::Vector2f{ VELOCITY, 0.f });
					break;
				case sf::Keyboard::Up:
					movableMapRect.move(sf::Vector2f{ 0.f, -VELOCITY });
					currentObjFallVelocity = 0.f;
					break;
				case sf::Keyboard::Left:
					movableMapRect.move(sf::Vector2f{ -VELOCITY, 0.f });
					break;
				case sf::Keyboard::Down:
					movableMapRect.move(sf::Vector2f{ 0.f, VELOCITY });
					break;
				case sf::Keyboard::Right:
					movableMapRect.move(sf::Vector2f{ VELOCITY, 0.f });
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
					movableMapRect.rotate(scrollDirection);
				}
				else
				{
					obj.rotate(scrollDirection);
				}
			}

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear(sf::Color::Black);

		auto objVertices = Engine::getVertices(&obj);

		sf::FloatRect bounds = obj.getGlobalBounds();
		std::vector<sf::Shape*> partsCollideCheck;
		
		for (auto& part : map)
		{
			if (bounds.intersects(part->getGlobalBounds()))
			{
				partsCollideCheck.push_back(part);
				part->setFillColor(sf::Color::Green);
			}
		}

		for (const auto& part : partsCollideCheck)
		{
			std::optional<Engine::CollisionResponse> response = Engine::checkCollide(objVertices, Engine::getVertices(part));

			if (response != std::nullopt)
			{
				sf::Vector2f MTV = response->MinimumTransitionVector;
				obj.setFillColor(sf::Color::Red);
				obj.move(MTV);
				currentObjFallVelocity /= 1.2f;
			}
		}

		window.draw(obj);
		window.draw(movableMapRect);
		window.draw(staticMapRect);

		// paint the rectangles white
		obj.setFillColor(sf::Color::White);
		movableMapRect.setFillColor(sf::Color::White);
		staticMapRect.setFillColor(sf::Color::White);
		
		obj.move({ 0.f, currentObjFallVelocity });

		window.display();
	}

	return 0;
}