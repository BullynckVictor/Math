#pragma once
#include "Point.h"
#include "Angle.h"

namespace math
{
	namespace detail
	{
		template<typename M, Tuple T, size_t C = 0>
		static constexpr void vector_magnitude_sq_helper(typename ranked_type<M, typename T::value_type>::higher& magnitude, const T& vector)
		{
			auto component = high_cast<M, typename T::value_type>(vector.get_component<C>());
			magnitude += component * component;
			if constexpr (C < T::dimensions - 1)
				vector_magnitude_sq_helper<M, T, C + 1>(magnitude, vector);
		}

		template<typename M, Tuple T, size_t C = 0>
		static constexpr M vector_magnitude_sq(const T& vector)
		{
			typename ranked_type<M, typename T::value_type>::higher magnitude = 0;
			vector_magnitude_sq_helper<M>(magnitude, vector);
			return static_cast<M>(magnitude);
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

		template<Tuple T, size_t C = 0>
		static constexpr typename T::value_type vector_dot(const T& a, const T& b)
		{
			auto value = a.get_component<C>() * b.get_component<C>();
			if constexpr (C < T::dimensions - 1)
				value += vector_dot<T, C + 1>(a, b);
			return value;
		}

		template<typename A, Tuple T>
		static Radians<A> vector_angle_helper(const T& a)
		{
			static_assert(T::dimensions == 2, "helper only used in 2 dimensions");
			return arctan<A>(static_cast<A>(a.get_component<0>()), static_cast<A>(a.get_component<1>()) );
		}

		template<typename T, typename F, typename... Args>
		static constexpr bool args_same_type()
		{
			bool same = std::is_same_v<T, F>;
			if constexpr (sizeof...(Args))
				same = same && args_same_type<T, Args...>();
			return same;
		}

		template<size_t D, typename T, typename... Args>
		concept component_args = sizeof...(Args) > 1 && sizeof...(Args) == D && args_same_type<T, Args...>();
	}

	template<size_t D, typename T>
	struct VectorBase : public Coordinates<D, T>
	{
		constexpr VectorBase() : Coordinates<D, T>() {}
		constexpr VectorBase(const T& value) : Coordinates<D, T>(value) {}
		constexpr VectorBase(const Coordinates<D, T>& coordinates) : Coordinates<D, T>(coordinates) {}
		constexpr VectorBase(Coordinates<D, T>&& coordinates) : Coordinates<D, T>(std::move(coordinates)) {}
		constexpr VectorBase(const Coordinates<D, T>& a, const Coordinates<D, T>& b) : Coordinates<D, T>(b - a) {}
		template<typename... Args>
		requires detail::component_args<D, T, Args...>
		constexpr VectorBase(const Args&... args) : Coordinates<D, T>(args...) {}

		template<typename M = T>
		constexpr M magnitude_sq() const { return detail::vector_magnitude_sq<M>(*this); }
		template<typename L = T>
		constexpr L length_sq() const { return magnitude_sq<L>(); }

		template<typename M = T>
		M magnitude() const { return sqrt(magnitude_sq<M>()); }
		template<typename L = T>
		L length() const { return sqrt(length_sq<L>()); }

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
		constexpr Vector(const T& value) : VectorBase<D, T>(value) {}
		constexpr Vector(const Coordinates<D, T>& coordinates) : VectorBase<D, T>(coordinates) {}
		constexpr Vector(Coordinates<D, T>&& coordinates) : VectorBase<D, T>(std::move(coordinates)) {}
		constexpr Vector(const Coordinates<D, T>& a, const Coordinates<D, T>& b) : VectorBase<D, T>(b - a) {}
	};

	template<typename T>
	struct Vector<1, T> : public VectorBase<1, T>
	{
		constexpr Vector() : VectorBase<1, T>() {}
		constexpr Vector(const T& x) : VectorBase<1, T>(x) {}
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
		constexpr Vector(const T& value) : VectorBase<2, T>(value) {}
		constexpr Vector(const T& x, const T& y) : VectorBase<2, T>(x, y) {}
		constexpr Vector(const Coordinates<2, T>& coordinates) : VectorBase<2, T>(coordinates) {}
		constexpr Vector(Coordinates<2, T>&& coordinates) : VectorBase<2, T>(std::move(coordinates)) {}
		constexpr Vector(const Coordinates<2, T>& a, const Coordinates<2, T>& b) : VectorBase<2, T>(b - a) {}

		template<typename A = double>
		Radians<A> angle() const { return detail::vector_angle_helper<A>(*this); }
	};

	template<typename T>
	struct Vector<3, T> : public VectorBase<3, T>
	{
		constexpr Vector() : VectorBase<3, T>() {}
		constexpr Vector(const T& value) : VectorBase<3, T>(value) {}
		constexpr Vector(const T& x, const T& y, const T& z) : VectorBase<3, T>(x, y, z) {}
		constexpr Vector(const Coordinates<3, T>& coordinates) : VectorBase<3, T>(coordinates) {}
		constexpr Vector(Coordinates<3, T>&& coordinates) : VectorBase<3, T>(std::move(coordinates)) {}
		constexpr Vector(const Coordinates<3, T>& a, const Coordinates<3, T>& b) : VectorBase<3, T>(b - a) {}
	};

	template<typename T>
	struct Vector<4, T> : public VectorBase<4, T>
	{
		constexpr Vector() : VectorBase<4, T>() {}
		constexpr Vector(const T& value) : VectorBase<4, T>(value) {}
		constexpr Vector(const T& x, const T& y, const T& z, const T& w) : VectorBase<4, T>(x, y, z, w) {}
		constexpr Vector(const Coordinates<4, T>& coordinates) : VectorBase<4, T>(coordinates) {}
		constexpr Vector(Coordinates<4, T>&& coordinates) : VectorBase<4, T>(std::move(coordinates)) {}
		constexpr Vector(const Coordinates<4, T>& a, const Coordinates<4, T>& b) : VectorBase<4, T>(b - a) {}
	};

	template<size_t D, typename T>
	static constexpr T dot(const Vector<D, T>& a, const Vector<D, T>& b)
	{
		return detail::vector_dot(a, b);
	}
	template<typename A = double, size_t D, typename T>
	static Radians<A> angle(const Vector<D, T>& a, const Vector<D, T>& b)
	{
		using highest = detail::ranked_type<A, T>::higher;
		return arccos<A>(static_cast<A>(static_cast<highest>(dot(a, b)) / (a.length<highest>() * b.length<highest>()))).angle;
	}
}