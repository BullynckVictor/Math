#pragma once
#include "Coordinates.h"

namespace math
{
	template<size_t D, typename T>
	struct Point : public Coordinates<D, T>
	{
		constexpr Point() : Coordinates<D, T>() {}
		constexpr Point(const T& value) : Coordinates<D, T>(value) {}
		constexpr Point(const Point& rhs) : Coordinates<D, T>(rhs) {}
		constexpr Point(Point&& rhs) noexcept : Coordinates<D, T>(std::move(rhs)) {}
	};

	template<typename T>
	struct Point<1, T> : public Coordinates<1, T>
	{
		constexpr Point() : Coordinates<1, T>() {}
		constexpr Point(const T& x) : Coordinates<1, T>(x) {}
		constexpr Point(const Point& rhs) : Coordinates<1, T>(rhs) {}
		constexpr Point(Point&& rhs) noexcept : Coordinates<1, T>(std::move(rhs)) {}
	};

	template<typename T>
	struct Point<2, T> : public Coordinates<2, T>
	{
		constexpr Point() : Coordinates<2, T>() {}
		constexpr Point(const T& value) : Coordinates<2, T>(value) {}
		constexpr Point(const T& x, const T& y) : Coordinates<2, T>(x, y) {}
		constexpr Point(const Point& rhs) : Coordinates<2, T>(rhs) {}
		constexpr Point(Point&& rhs) noexcept : Coordinates<2, T>(std::move(rhs)) {}
	};

	template<typename T>
	struct Point<3, T> : public Coordinates<3, T>
	{
		constexpr Point() : Coordinates<3, T>() {}
		constexpr Point(const T& value) : Coordinates<3, T>(value) {}
		constexpr Point(const T& x, const T& y, const T& z) : Coordinates<3, T>(x, y, z) {}
		constexpr Point(const Point& rhs) : Coordinates<3, T>(rhs) {}
		constexpr Point(Point&& rhs) noexcept : Coordinates<3, T>(std::move(rhs)) {}
	};

	template<typename T>
	struct Point<4, T> : public Coordinates<4, T>
	{
		constexpr Point() : Coordinates<4, T>() {}
		constexpr Point(const T& value) : Coordinates<4, T>(value) {}
		constexpr Point(const T& x, const T& y, const T& z, const T& w) : Coordinates<4, T>(x, y, z, w) {}
		constexpr Point(const Point& rhs) : Coordinates<4, T>(rhs) {}
		constexpr Point(Point&& rhs) noexcept : Coordinates<4, T>(std::move(rhs)) {}
	};
}