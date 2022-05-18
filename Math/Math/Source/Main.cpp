#include <iostream>
#include "Vector.h"

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
	std::cout << A<(int)foo()>::value << '\n';
	Vector<2, float> vector;
	vector.normalise();

	std::cout << distance(Point<2, float>(5, 3), Point<2, float>(1, 7)) << '\n';
	std::cout << vector.x << ", " << vector.y << '\n';
	std::cin.ignore();
}