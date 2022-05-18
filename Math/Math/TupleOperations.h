#pragma once
#include "Tuple.h"

namespace math
{
	namespace detail
	{
		template<Tuple T, size_t C = 0>
		static constexpr bool tuple_equals(const T& a, const T& b)
		{
			bool equals = a.get_component<C>() == b.get_component<C>();
			if constexpr (C < T::dimensions - 1)
				equals = equals && tuple_equals<T, C + 1>(a, b);
			return equals;
		}

		template<Tuple T, size_t C = 0>
		static constexpr bool tuple_not_equals(const T& a, const T& b)
		{
			bool not_equals = a.get_component<C>() != b.get_component<C>();
			if constexpr (C < T::dimensions - 1)
				not_equals = not_equals || tuple_not_equals<T, C + 1>(a, b);
			return not_equals;
		}
	}

	template<Tuple T>
	static constexpr bool operator== (const T& a, const T& b)
	{
		return detail::tuple_equals(a, b);
	}
	template<Tuple T>
	static constexpr bool operator!= (const T& a, const T& b)
	{
		return detail::tuple_not_equals(a, b);
	}

	namespace detail
	{
		template<Tuple T>
		struct TupleAddition
		{
			using value_type = typename T::value_type;
			static constexpr value_type  operation(const value_type& a, const value_type& b) { return a + b; }
			static constexpr value_type& operation_self(value_type& a, const value_type& b) { return a += b; }
		};
		template<Tuple T>
		struct TupleSubtraction
		{
			using value_type = typename T::value_type;
			static constexpr value_type  operation(const value_type& a, const value_type& b) { return a - b; }
			static constexpr value_type& operation_self(value_type& a, const value_type& b) { return a -= b; }
		};
		template<Tuple T>
		struct TupleDivision
		{
			using value_type = typename T::value_type;
			static constexpr value_type  operation(const value_type& a, const value_type& b) { return a / b; }
			static constexpr value_type& operation_self(value_type& a, const value_type& b) { return a /= b; }
		};
		template<Tuple T>
		struct TupleMultiplication
		{
			using value_type = typename T::value_type;
			static constexpr value_type  operation(const value_type& a, const value_type& b) { return a * b; }
			static constexpr value_type& operation_self(value_type& a, const value_type& b) { return a *= b; }
		};

		template<typename O, Tuple T, size_t C = 0>
		static constexpr void tuple_operation(T& out, const T& a, const T& b)
		{
			out.get_component<C>() = O::operation(a.get_component<C>(), b.get_component<C>());
			if constexpr (C < T::dimensions - 1)
				tuple_operation<O, T, C + 1>(out, a, b);
		}
		template<typename O, Tuple T, size_t C = 0>
		static constexpr void tuple_operation_self(T& a, const T& b)
		{
			O::operation_self(a.get_component<C>(), b.get_component<C>());
			if constexpr (C < T::dimensions - 1)
				tuple_operation_self<O, T, C + 1>(a, b);
		}
		template<typename O, Tuple T, size_t C = 0>
		static constexpr void tuple_operation_scalar(T& out, const T& tuple, const typename T::value_type& scalar)
		{
			out.get_component<C>() = O::operation(tuple.get_component<C>(), scalar);
			if constexpr (C < T::dimensions - 1)
				tuple_operation_scalar<O, T, C + 1>(out, tuple, scalar);
		}
		template<typename O, Tuple T, size_t C = 0>
		static constexpr void tuple_operation_scalar_self(T& tuple, const typename T::value_type& scalar)
		{
			O::operation_self(tuple.get_component<C>(), scalar);
			if constexpr (C < T::dimensions - 1)
				tuple_operation_scalar_self<O, T, C + 1>(tuple, scalar);
		}
	}

	template<Tuple T>
	static constexpr T operator+ (const T& a, const T& b)
	{
		T result;
		detail::tuple_operation<detail::TupleAddition<T>>(result, a, b);
		return result;
	}
	template<Tuple T>
	static constexpr T operator- (const T& a, const T& b)
	{
		T result;
		detail::tuple_operation<detail::TupleSubtraction<T>>(result, a, b);
		return result;
	}

	template<Tuple T>
	static constexpr T& operator+= (T& a, const T& b)
	{
		detail::tuple_operation_self<detail::TupleAddition<T>>(a, b);
		return a;
	}
	template<Tuple T>
	static constexpr T& operator-= (T& a, const T& b)
	{
		detail::tuple_operation_self<detail::TupleSubtraction<T>>(a, b);
		return a;
	}

	template<Tuple T>
	static constexpr T operator* (const T& tuple, const typename T::value_type& scalar)
	{
		T result;
		detail::tuple_operation_scalar<detail::TupleMultiplication<T>>(result, tuple, scalar);
		return result;
	}
	template<Tuple T>
	static constexpr T operator/ (const T& tuple, const typename T::value_type& scalar)
	{
		T result;
		detail::tuple_operation_scalar<detail::TupleDivision<T>>(result, tuple, scalar);
		return result;
	}
	template<Tuple T>
	static constexpr T operator* (const typename T::value_type& scalar, const T& tuple)
	{
		T result;
		detail::tuple_operation_scalar<detail::TupleMultiplication<T>>(result, tuple, scalar);
		return result;
	}
	template<Tuple T>
	static constexpr T& operator*= (T& tuple, const typename T::value_type& scalar)
	{
		detail::tuple_operation_scalar_self<detail::TupleMultiplication<T>>(tuple, scalar);
		return tuple;
	}
	template<Tuple T>
	static constexpr T& operator/= (T& tuple, const typename T::value_type& scalar)
	{
		detail::tuple_operation_scalar_self<detail::TupleDivision<T>>(tuple, scalar);
		return tuple;
	}

	namespace detail
	{
		template<Tuple T1, SameTuple<T1::dimensions, typename T1::value_type> T2, size_t C = 0>
		static constexpr typename T1::value_type tuple_distance_sq(const T1& a, const T2& b)
		{
			auto distance = b.get_component<C>() - a.get_component<C>();
			distance = distance * distance;
			if constexpr (C < T1::dimensions - 1)
				distance += tuple_distance_sq<T1, T2, C + 1>(a, b);
			return distance;
		}
	}

	template<Tuple T1, SameTuple<T1::dimensions, typename T1::value_type> T2>
	static constexpr typename T1::value_type distance_sq(const T1& a, const T2& b)
	{
		return detail::tuple_distance_sq(a, b);
	}

	template<typename T>
	requires std::is_arithmetic_v<T>
	static T sqrt(const T& value)
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			if constexpr (std::is_same_v<typename std::remove_cvref<T>::type, float>)
				return ::sqrtf(value);
			else if constexpr (std::is_same_v<typename std::remove_cvref<T>::type, double>)
				return ::sqrt(value);
			else
				return ::sqrtl(static_cast<long double>(value));
		}
		else
			return ::sqrt(static_cast<double>(value));
	}

	template<Tuple T1, SameTuple<T1::dimensions, typename T1::value_type> T2>
	static typename T1::value_type distance(const T1& a, const T2& b)
	{
		return sqrt(distance_sq(a, b));
	}
}