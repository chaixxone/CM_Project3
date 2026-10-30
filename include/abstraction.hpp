#pragma once

#include <vector>

namespace Engine
{
	struct Vector2f
	{
		Vector2f() : x(0.f), y(0.f) {}
		Vector2f(float x, float y) : x(x), y(y) {}

		float x;
		float y;
	};

	class IRenderable
	{
	public:
		virtual void Render() = 0;
	};

	class IShape
	{
	public:
		virtual std::vector<Vector2f> GetVertices() = 0;
		virtual size_t GetPointCount() = 0;
		virtual void Move(Vector2f& direction) = 0;
	};
} // namespace Engine