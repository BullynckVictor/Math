#pragma once
#include <type_traits>

namespace math
{
	template<size_t D, typename T>
	requires std::is_arithmetic_v<T>
	struct TupleBase
	{
		using value_type = T;
		static constexpr size_t dimensions = D;
	};

	template<typename T>
	concept Tuple = std::is_base_of_v<TupleBase<T::dimensions, typename T::value_type>, T>;

	template<typename T, size_t D, typename T2>
	concept SameTuple = Tuple<T> && std::is_same_v<typename T::value_type, T2> && T::dimensions == D;
}