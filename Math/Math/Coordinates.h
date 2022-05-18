#pragma once
#include <utility>
#include "Tuple.h"

namespace math
{
	template<size_t D, typename T>
	struct Coordinates : public TupleBase<D, T>
	{
	public:
		constexpr Coordinates() : components() {}
		constexpr Coordinates(const T& value) { *this = value; }
		constexpr Coordinates(const Coordinates& rhs) { copy_components<0>(rhs); }
		constexpr Coordinates(Coordinates&& rhs) noexcept { move_components<0>(rhs); }

		constexpr Coordinates& operator= (const Coordinates& rhs)
		{
			copy_components<0>(rhs);
			return *this;
		}
		constexpr Coordinates& operator= (Coordinates&& rhs) noexcept
		{
			move_components<0>(rhs);
			return *this;
		}
		constexpr Coordinates& operator= (const T& value)
		{
			copy_components<0>(value);
			return *this;
		}

		template<size_t C>
		constexpr const T& get_component() const { static_assert(C < D, "component index out of range"); return components[C]; }
		template<size_t C>
		constexpr T& get_component() { static_assert(C < D, "component index out of range"); return components[C]; }

		T components[D];

	private:
		template<size_t C>
		constexpr void copy_components(const T& value)
		{
			components[C] = value;
			if constexpr (C < D - 1)
				copy_components<C + 1>(value);
		}

		template<size_t C>
		constexpr void copy_components(const Coordinates& coordinates)
		{
			components[C] = coordinates.components[C];
			if constexpr (C < D - 1)
				copy_components<C + 1>(coordinates);
		}

		template<size_t C>
		constexpr void move_components(Coordinates& coordinates) noexcept
		{
			components[C] = std::move(coordinates.components[C]);
			if constexpr (C < D - 1)
				copy_components<C + 1>(coordinates);
		}
	};

	template<typename T>
	struct Coordinates<1, T> : public TupleBase<1, T>
	{
		constexpr Coordinates() : x(0) {}
		constexpr Coordinates(const T& x) : x(x) {}
		constexpr Coordinates(const Coordinates& rhs) : x(rhs.x) {}
		constexpr Coordinates(Coordinates&& rhs) noexcept : x(std::move(rhs.x)) {}

		constexpr Coordinates& operator= (const Coordinates& rhs)
		{
			x = rhs.x;
			return *this;
		}
		constexpr Coordinates& operator= (Coordinates&& rhs) noexcept
		{
			x = std::move(rhs.x);
			return *this;
		}
		constexpr Coordinates& operator= (const T& x)
		{
			this->x = x;
			return *this;
		}

		template<size_t C>
		constexpr const T& get_component() const { static_assert(C < 1, "component index out of range"); }
		template<> constexpr const T& get_component<0>() const { return x; }

		template<size_t C>
		constexpr T& get_component() { static_assert(C < 1, "component index out of range"); }
		template<> constexpr T& get_component<0>() { return x; }

		T x;
	};

	template<typename T>
	struct Coordinates<2, T> : public TupleBase<2, T>
	{
		constexpr Coordinates() : x(0), y(0) {}
		constexpr Coordinates(const T& value) : x(value), y(value) {}
		constexpr Coordinates(const T& x, const T& y) : x(x), y(y) {}
		constexpr Coordinates(const Coordinates& rhs) : x(rhs.x), y(rhs.y) {}
		constexpr Coordinates(Coordinates&& rhs) noexcept : x(std::move(rhs.x)), y(std::move(rhs.y)) {}

		constexpr Coordinates& operator= (const Coordinates& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			return *this;
		}
		constexpr Coordinates& operator= (Coordinates&& rhs) noexcept
		{
			x = std::move(rhs.x);
			y = std::move(rhs.y);
			return *this;
		}
		constexpr Coordinates& operator= (const T& value)
		{
			x = value;
			y = value;
			return *this;
		}

		template<size_t C>
		constexpr const T& get_component() const { static_assert(C < 2, "component index out of range"); }
		template<> constexpr const T& get_component<0>() const { return x; }
		template<> constexpr const T& get_component<1>() const { return y; }

		template<size_t C>
		constexpr T& get_component() { static_assert(C < 2, "component index out of range"); }
		template<> constexpr T& get_component<0>() { return x; }
		template<> constexpr T& get_component<1>() { return y; }

		T x;
		T y;
	};

	template<typename T>
	struct Coordinates<3, T> : public TupleBase<3, T>
	{
		constexpr Coordinates() : x(0), y(0), z(0) {}
		constexpr Coordinates(const T& value) : x(value), y(value), z(value) {}
		constexpr Coordinates(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}
		constexpr Coordinates(const Coordinates& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}
		constexpr Coordinates(Coordinates&& rhs) noexcept : x(std::move(rhs.x)), y(std::move(rhs.y)), z(std::move(rhs.z)) {}

		constexpr Coordinates& operator= (const Coordinates& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			return *this;
		}
		constexpr Coordinates& operator= (Coordinates&& rhs) noexcept
		{
			x = std::move(rhs.x);
			y = std::move(rhs.y);
			z = std::move(rhs.z);
			return *this;
		}
		constexpr Coordinates& operator= (const T& value)
		{
			x = value;
			y = value;
			z = value;
			return *this;
		}

		template<size_t C>
		constexpr const T& get_component() const { static_assert(C < 3, "component index out of range"); }
		template<> constexpr const T& get_component<0>() const { return x; }
		template<> constexpr const T& get_component<1>() const { return y; }
		template<> constexpr const T& get_component<2>() const { return z; }

		template<size_t C>
		constexpr T& get_component() { static_assert(C < 3, "component index out of range"); }
		template<> constexpr T& get_component<0>() { return x; }
		template<> constexpr T& get_component<1>() { return y; }
		template<> constexpr T& get_component<2>() { return z; }

		T x;
		T y;
		T z;
	};

	template<typename T>
	struct Coordinates<4, T> : public TupleBase<4, T>
	{
		constexpr Coordinates() : x(0), y(0), z(0), w(0) {}
		constexpr Coordinates(const T& value) : x(value), y(value), z(value), w(value) {}
		constexpr Coordinates(const T& x, const T& y, const T& z, const T& w) : x(x), y(y), z(z), w(w) {}
		constexpr Coordinates(const Coordinates& rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) {}
		constexpr Coordinates(Coordinates&& rhs) noexcept : x(std::move(rhs.x)), y(std::move(rhs.y)), z(std::move(rhs.z)), w(std::move(rhs.w)) {}

		constexpr Coordinates& operator= (const Coordinates& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;
			return *this;
		}
		constexpr Coordinates& operator= (Coordinates&& rhs) noexcept
		{
			x = std::move(rhs.x);
			y = std::move(rhs.y);
			z = std::move(rhs.z);
			w = std::move(rhs.w);
			return *this;
		}
		constexpr Coordinates& operator= (const T& value)
		{
			x = value;
			y = value;
			z = value;
			w = value;
			return *this;
		}

		template<size_t C> 
		constexpr const T& get_component() const { static_assert(C < 4, "component index out of range"); }
		template<> constexpr const T& get_component<0>() const { return x; }
		template<> constexpr const T& get_component<1>() const { return y; }
		template<> constexpr const T& get_component<2>() const { return z; }
		template<> constexpr const T& get_component<3>() const { return w; }

		template<size_t C>
		constexpr T& get_component() { static_assert(C < 4, "component index out of range"); }
		template<> constexpr T& get_component<0>() { return x; }
		template<> constexpr T& get_component<1>() { return y; }
		template<> constexpr T& get_component<2>() { return z; }
		template<> constexpr T& get_component<3>() { return w; }

		T x;
		T y;
		T z;
		T w;
	};
}

#include "TupleOperations.h"