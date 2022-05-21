#pragma once

namespace math
{
	template<typename T>
	struct constants
	{
		static constexpr T pi = (T)3.14159265358979323846l;
		static constexpr T eulers = (T)0.5772156649015328606065120900824024310421l;

		static constexpr T inverse_pi = (T)1 / pi;
		static constexpr T inverse_eulers = (T)1 / eulers;
	};
}