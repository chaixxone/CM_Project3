#include <compare>

#include <SFML/System/Vector2.hpp>

namespace Engine
{
	/**
	* @brief Defines a projection onto a normal in 1D dimention 
	* and contains the position of point that is projected
	*/
	class Projection
	{
	public:
		/**
		* @brief Default constructor
		*/
		Projection() : _projectionOntoNormal(0.f), _projectedPoint(sf::Vector2f{ 0.f, 0.f }) {}

		/**
		* @brief Creates Projection object
		* @param projection: value of 1D projection onto a normal
		* @param point: the point that is projected onto a normal
		*/
		Projection(float projection, const sf::Vector2f& point)
			: _projectionOntoNormal(projection), _projectedPoint(point) {}

		sf::Vector2f GetPoint() const noexcept
		{
			return _projectedPoint;
		}

		float GetProjectionValue() const noexcept
		{
			return _projectionOntoNormal;
		}

		void operator=(const Projection& other)
		{
			_projectionOntoNormal = other._projectionOntoNormal;
			_projectedPoint = other._projectedPoint;
		}

		auto operator<=>(const Projection& other) const
		{
			return _projectionOntoNormal <=> other._projectionOntoNormal;
		}

		/**
		* @brief returns difference between projection values
		* @param other: right projection
		* @returns difference in float
		*/
		float operator-(const Projection& other)
		{
			return _projectionOntoNormal - other._projectionOntoNormal;
		}

	private:
		float _projectionOntoNormal;
		sf::Vector2f _projectedPoint;
	};
} // namespace Engine