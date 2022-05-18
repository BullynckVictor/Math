#pragma once
#include "Point.h"

namespace math
{
	namespace detail
	{
		template<Tuple T, size_t C = 0>
		static constexpr typename T::value_type vector_magnitude_sq(const T& vector)
		{
			auto magnitude = vector.get_component<C>() * vector.get_component<C>();
			if constexpr (C < T::dimensions - 1)
				magnitude += vector_magnitude_sq<T, C + 1>(vector);
			return magnitude;
		}

		template<Tuple T, size_t C = 0>
		static constexpr T vector_normalised(T& out, const T& vector, const typename T::value_type& length)
		{
			out.get_component<C>() = vector.get_component<C>() / length;
			if constexpr (C < T::dimensions - 1)
				vector_normalised<T, C + 1>(out, vector, length);
		}
		template<Tuple T, size_t C = 0>
		static constexpr T vector_normalised_inv(T& out, const T& vector, const typename T::value_type& inv_length)
		{
			out.get_component<C>() = vector.get_component<C>() * inv_length;
			if constexpr (C < T::dimensions - 1)
				vector_normalised_inv<T, C + 1>(out, vector, inv_length);
		}
	}

	template<size_t D, typename T>
	struct VectorBase : public Coordinates<D, T>
	{
		constexpr VectorBase() : Coordinates<D, T>() {}
		constexpr VectorBase(const T& value) : Coordinates<D, T>(value) {}
		constexpr VectorBase(const Coordinates<D, T>& coordinates) : Coordinates<D, T>(coordinates) {}
		constexpr VectorBase(Coordinates<D, T>&& coordinates) : Coordinates<D, T>(std::move(coordinates)) {}
		constexpr VectorBase(const Coordinates<D, T>& a, const Coordinates<D, T>& b) : Coordinates<D, T>(b - a) {}

		constexpr T magnitude_sq() const { return detail::vector_magnitude_sq(*this); }
		constexpr T length_sq() const { return magnitude_sq(); }

		T magnitude() const { return sqrt(magnitude_sq()); }
		T length() const { return sqrt(length_sq()); }

		VectorBase normalised() const 
		{
			VectorBase out;
			if constexpr (std::is_integral_v<T>)
				detail::vector_normalised(out, *this, length());
			else
				detail::vector_normalised_inv(out, *this, (T)1 / length());
			return out;
		}
		void normalise()
		{
			if constexpr (std::is_integral_v<T>)
				*this /= length();
			else
				*this *= ((T)1 / length());
		}
	};

	template<size_t D, typename T>
	struct Vector : public VectorBase<D, T>
	{
		constexpr Vector() : VectorBase<D, T>() {}
		constexpr Vector(const T& value) : Coordinates<D, T>(value) {}
		constexpr Vector(const Coordinates<D, T>& coordinates) : VectorBase<D, T>(coordinates) {}
		constexpr Vector(Coordinates<D, T>&& coordinates) : VectorBase<D, T>(std::move(coordinates)) {}
		constexpr Vector(const Coordinates<D, T>& a, const Coordinates<D, T>& b) : VectorBase<D, T>(b - a) {}
	};

	template<typename T>
	struct Vector<1, T> : public VectorBase<1, T>
	{
		constexpr Vector() : VectorBase<1, T>() {}
		constexpr Vector(const T& x) : Coordinates<1, T>(x) {}
		constexpr Vector(const Coordinates<1, T>& coordinates) : VectorBase<1, T>(coordinates) {}
		constexpr Vector(Coordinates<1, T>&& coordinates) : VectorBase<1, T>(std::move(coordinates)) {}
		constexpr Vector(const Coordinates<1, T>& a, const Coordinates<1, T>& b) : VectorBase<1, T>(b - a) {}

		T magnitude() const { return get_component<0>(); }
		T length() const { return get_component<0>(); }
	};

	template<typename T>
	struct Vector<2, T> : public VectorBase<2, T>
	{
		constexpr Vector() : VectorBase<2, T>() {}
		constexpr Vector(const T& value) : Coordinates<2, T>(value) {}
		constexpr Vector(const T& x, const T& y) : Coordinates<2, T>(x, y) {}
		constexpr Vector(const Coordinates<2, T>& coordinates) : VectorBase<2, T>(coordinates) {}
		constexpr Vector(Coordinates<2, T>&& coordinates) : VectorBase<2, T>(std::move(coordinates)) {}
		constexpr Vector(const Coordinates<2, T>& a, const Coordinates<2, T>& b) : VectorBase<2, T>(b - a) {}
	};

	template<typename T>
	struct Vector<3, T> : public VectorBase<3, T>
	{
		constexpr Vector() : VectorBase<3, T>() {}
		constexpr Vector(const T& value) : Coordinates<3, T>(value) {}
		constexpr Vector(const T& x, const T& y, const T& z) : Coordinates<3, T>(x, y, z) {}
		constexpr Vector(const Coordinates<3, T>& coordinates) : VectorBase<3, T>(coordinates) {}
		constexpr Vector(Coordinates<3, T>&& coordinates) : VectorBase<3, T>(std::move(coordinates)) {}
		constexpr Vector(const Coordinates<3, T>& a, const Coordinates<3, T>& b) : VectorBase<3, T>(b - a) {}
	};

	template<typename T>
	struct Vector<4, T> : public VectorBase<4, T>
	{
		constexpr Vector() : VectorBase<4, T>() {}
		constexpr Vector(const T& value) : Coordinates<4, T>(value) {}
		constexpr Vector(const T& x, const T& y, const T& z, const T& w) : Coordinates<4, T>(x, y, z, w) {}
		constexpr Vector(const Coordinates<4, T>& coordinates) : VectorBase<4, T>(coordinates) {}
		constexpr Vector(Coordinates<4, T>&& coordinates) : VectorBase<4, T>(std::move(coordinates)) {}
		constexpr Vector(const Coordinates<4, T>& a, const Coordinates<4, T>& b) : VectorBase<4, T>(b - a) {}
	};
}