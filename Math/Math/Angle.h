#pragma once
#include "Constants.h"
#include <type_traits>
#include <cmath>

namespace math
{
	namespace detail
	{
		template<typename T1, typename T2, bool T1L>
		struct both_same_type_t1_lesser {};
		template<typename T1, typename T2>
		struct both_same_type_t1_lesser<T1, T2, false>
		{
			using higher = T1;
			using lower  = T2;
		};
		template<typename T1, typename T2>
		struct both_same_type_t1_lesser<T1, T2, true>
		{
			using higher = T2;
			using lower  = T1;
		};

		template<typename T1, typename T2, bool T1F>
		struct one_floating_type_t1_floating {};
		template<typename T1, typename T2>
		struct one_floating_type_t1_floating<T1, T2, true>
		{
			using higher = T1;
			using lower  = T2;
		};
		template<typename T1, typename T2>
		struct one_floating_type_t1_floating<T1, T2, false>
		{
			using higher = T2;
			using lower  = T1;
		};

		template<typename T1, typename T2, bool F>
		struct one_floating_type {};
		template<typename T1, typename T2>
		struct one_floating_type<T1, T2, true>
		{
			using higher = typename one_floating_type_t1_floating<T1, T2, std::is_floating_point_v<T1>>::higher;
			using lower  = typename one_floating_type_t1_floating<T1, T2, std::is_floating_point_v<T1>>::lower;
		};
		template<typename T1, typename T2>
		struct one_floating_type<T1, T2, false>
		{
			using higher = typename both_same_type_t1_lesser<T1, T2, sizeof(T1) < sizeof(T2)>::higher;
			using lower  = typename both_same_type_t1_lesser<T1, T2, sizeof(T1) < sizeof(T2)>::lower;
		};

		template<typename T1, typename T2, bool F>
		struct both_floating_type {};
		template<typename T1, typename T2>
		struct both_floating_type<T1, T2, true>
		{
			using higher = typename both_same_type_t1_lesser<T1, T2, sizeof(T1) < sizeof(T2)>::higher;
			using lower  = typename both_same_type_t1_lesser<T1, T2, sizeof(T1) < sizeof(T2)>::lower;
		};
		template<typename T1, typename T2>
		struct both_floating_type<T1, T2, false>
		{
			using higher = typename one_floating_type<T1, T2, std::is_floating_point_v<T1> || std::is_floating_point_v<T2>>::higher;
			using lower  = typename one_floating_type<T1, T2, std::is_floating_point_v<T1> || std::is_floating_point_v<T2>>::lower;
		};

		template<typename T1, typename T2>
		struct ranked_type
		{
			using higher = typename both_floating_type<T1, T2, std::is_floating_point_v<T1> && std::is_floating_point_v<T2>>::higher;
			using lower  = typename both_floating_type<T1, T2, std::is_floating_point_v<T1> && std::is_floating_point_v<T2>>::lower;
		};

		template<typename T1, typename T2, typename T3>
		requires std::is_same_v<T1, T3> || std::is_same_v<T2, T3>
		static constexpr typename ranked_type<T1, T2>::higher high_cast(const T3& value)
		{
			return static_cast<typename ranked_type<T1, T2>::higher>(value);
		}
	}

	template<typename T>
	requires std::is_floating_point_v<T>
	struct Radians
	{
		constexpr Radians() : angle() {}
		constexpr Radians(const T& angle) : angle(angle) {}
		constexpr Radians(T&& angle) : angle(std::move(angle)) {}

		T angle;
	};

	template<typename T>
	requires std::is_arithmetic_v<T>
	struct Degrees
	{
		constexpr Degrees() : angle() {}
		constexpr Degrees(const T& angle) : angle(angle) {}
		constexpr Degrees(T&& angle) : angle(std::move(angle)) {}
		template<typename T2 = T>
		requires std::is_floating_point_v<T2>
		constexpr Degrees(const Radians<T2>& angle) : angle(static_cast<T>(detail::high_cast<T, T2>(angle.angle) * (typename detail::ranked_type<T, T2>::higher)180 * constants<typename detail::ranked_type<T, T2>::higher>::inverse_pi)) {}

		template<typename T2 = T>
		requires std::is_floating_point_v<T2>
		constexpr operator Radians<T2>() const { return static_cast<T2>(detail::high_cast<T, T2>(angle) * constants<typename detail::ranked_type<T, T2>::higher>::pi / (typename detail::ranked_type<T, T2>::higher)180);  }

		T angle;
	};

	template<typename T>
	requires std::is_arithmetic_v<T>
	struct PiFactor
	{
		constexpr PiFactor() : angle() {}
		constexpr PiFactor(const T& angle) : angle(angle) {}
		constexpr PiFactor(T&& angle) : angle(std::move(angle)) {}
		template<typename T2 = T>
		requires std::is_floating_point_v<T2>
		constexpr PiFactor(const Radians<T2>& angle) : angle(static_cast<T>(detail::high_cast<T, T2>(angle.angle) * constants<typename detail::ranked_type<T, T2>::higher>::inverse_pi)) {}
		template<typename T2 = T>
		requires std::is_arithmetic_v<T2>
		constexpr PiFactor(const Degrees<T2>& angle) : angle(static_cast<T>(detail::high_cast<T, T2>(angle.angle) / (typename detail::ranked_type<T, T2>::higher)180)) {}

		template<typename T2 = T>
		requires std::is_floating_point_v<T2>
		constexpr operator Radians<T2>() const { return static_cast<T2>(detail::high_cast<T, T2>(angle) * constants<typename detail::ranked_type<T, T2>::higher>::pi); }
		template<typename T2 = T>
		requires std::is_arithmetic_v<T2>
		constexpr operator Degrees<T2>() const { return static_cast<T2>(detail::high_cast<T, T2>(angle) * (typename detail::ranked_type<T, T2>::higher)180); }

		T angle;
	};

	namespace detail
	{
		template<typename T>
		struct GoniometricFunctions
		{};

		template<>
		struct GoniometricFunctions<float>
		{
			static float acos(float x)				{ return ::acosf(x);}
			static float asin(float x)				{ return ::asinf(x);}
			static float atan(float x)				{ return ::atanf(x);}
			static float atan2(float y, float x)	{ return ::atan2f(y, x);}
			static float cos(float radians)			{ return ::cosf(radians);}
			static float sin(float radians)			{ return ::sinf(radians);}
			static float tan(float radians)			{ return ::tanf(radians);}
			static float cosh(float x)				{ return ::coshf(x);}
			static float sinh(float x)				{ return ::sinhf(x);}
			static float tanh(float x)				{ return ::tanhf(x);}
		};
		template<>
		struct GoniometricFunctions<double>
		{
			static double acos(double x)				{ return ::acos(x);}
			static double asin(double x)				{ return ::asin(x);}
			static double atan(double x)				{ return ::atan(x);}
			static double atan2(double y, double x)		{ return ::atan2(y, x);}
			static double cos(double radians)			{ return ::cos(radians);}
			static double sin(double radians)			{ return ::sin(radians);}
			static double tan(double radians)			{ return ::tan(radians);}
			static double cosh(double x)				{ return ::cosh(x);}
			static double sinh(double x)				{ return ::sinh(x);}
			static double tanh(double x)				{ return ::tanh(x);}
		};
		template<>
		struct GoniometricFunctions<long double>
		{
			static long double acos(long double x)						{ return ::acosl(x);}
			static long double asin(long double x)						{ return ::asinl(x);}
			static long double atan(long double x)						{ return ::atanl(x);}
			static long double atan2(long double y, long double x)		{ return ::atan2l(y, x);}
			static long double cos(long double radians)					{ return ::cosl(radians);}
			static long double sin(long double radians)					{ return ::sinl(radians);}
			static long double tan(long double radians)					{ return ::tanl(radians);}
			static long double cosh(long double x)						{ return ::coshl(x);}
			static long double sinh(long double x)						{ return ::sinhl(x);}
			static long double tanh(long double x)						{ return ::tanhl(x);}
		};
	}

	template<typename T = double>
	requires std::is_floating_point_v<T>
	static T sin(const Radians<T>& angle)
	{
		return detail::GoniometricFunctions<typename std::remove_cvref<T>::type>::sin(angle.angle);
	}
	template<typename T = double>
	requires std::is_floating_point_v<T>
	static T cos(const Radians<T>& angle)
	{
		return detail::GoniometricFunctions<typename std::remove_cvref<T>::type>::cos(angle.angle);
	}
	template<typename T = double>
	requires std::is_floating_point_v<T>
	static T tan(const Radians<T>& angle)
	{
		return detail::GoniometricFunctions<typename std::remove_cvref<T>::type>::tan(angle.angle);
	}
	template<typename T = double>
	requires std::is_floating_point_v<T>
	static Radians<T> arccos(const T& x)
	{
		return detail::GoniometricFunctions<typename std::remove_cvref<T>::type>::acos(x);
	}
	template<typename T = double>
	requires std::is_floating_point_v<T>
	static Radians<T> arcsin(const T& x)
	{
		return detail::GoniometricFunctions<typename std::remove_cvref<T>::type>::asin(x);
	}
	template<typename T = double>
	requires std::is_floating_point_v<T>
	static Radians<T> arctan(const T& x)
	{
		return detail::GoniometricFunctions<typename std::remove_cvref<T>::type>::atan(x);
	}
	template<typename T = double>
	requires std::is_floating_point_v<T>
	static Radians<T> arctan(const T& x, const T& y)
	{
		return detail::GoniometricFunctions<typename std::remove_cvref<T>::type>::atan2(y, x);
	}
	template<typename T = double>
	requires std::is_floating_point_v<T>
	static Radians<T> arctan2(const T& x, const T& y)
	{
		return detail::GoniometricFunctions<typename std::remove_cvref<T>::type>::atan2(y, x);
	}

	static double sin(const Radians<double>& angle)
	{
		return detail::GoniometricFunctions<double>::sin(angle.angle);
	}
	static double cos(const Radians<double>& angle)
	{
		return detail::GoniometricFunctions<double>::cos(angle.angle);
	}
	static double tan(const Radians<double>& angle)
	{
		return detail::GoniometricFunctions<double>::tan(angle.angle);
	}
}