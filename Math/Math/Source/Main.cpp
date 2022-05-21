#include <iostream>
#include "Vector.h"
#include "Angle.h"

using namespace math;

static constexpr float foo()
{
	Point<2, float> point1(5, 4);
	Point<2, float> point2(3, 4);

	VectorBase<1, float> vector(3);

	return vector.magnitude_sq();
}

template<int a>
struct A
{
	static constexpr int value = a;
};

int main()
{
	Radians<double> radians;
	Degrees<int> degrees;
	PiFactor<double> pifactor;

	while (true)
	{
		Vector<2, float> a;
		Vector<2, float> b;
		std::cout << "a.x:      ";
		std::cin >> a.x;
		std::cout << "a.y:      ";
		std::cin >> a.y;
		std::cout << "b.x:      ";
		std::cin >> b.x;
		std::cout << "b.y:      ";
		std::cin >> b.y;
		std::cout << "angle:    " << Degrees<int>(angle(a, b)).angle << "\n\n";
	}

	std::cin.ignore();
	std::cin.ignore();
}