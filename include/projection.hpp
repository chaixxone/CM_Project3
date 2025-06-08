#include <compare>

#include <SFML/System/Vector2.hpp>

namespace Engine
{
	/**
	* @brief Defines a projection onto a normal in 1D dimention 
	* and contains the index of the point that is projected
	*/
	class Projection
	{
	public:
		/**
		* @brief Default constructor
		*/
		Projection() : _projectionOntoNormal(0.f), _projectedPointIndex(0) {}

		/**
		* @brief Creates Projection object
		* @param projection: value of 1D projection onto a normal
		* @param point: an index of the point that is projected onto a normal
		*/
		Projection(float projection, size_t pointIndex)
			: _projectionOntoNormal(projection), _projectedPointIndex(pointIndex) {}

		size_t GetPointIndex() const noexcept
		{
			return _projectedPointIndex;
		}

		float GetProjectionValue() const noexcept
		{
			return _projectionOntoNormal;
		}

		void operator=(const Projection& other)
		{
			_projectionOntoNormal = other._projectionOntoNormal;
			_projectedPointIndex = other._projectedPointIndex;
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
		size_t _projectedPointIndex;
	};
} // namespace Engine