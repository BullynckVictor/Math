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

template<typename T>
void prompt(const char* prompt, T& x)
{
	std::cout << prompt;
	std::cin >> x;
}

#define print(x) std::cout << #x << ": " << x << '\n'

int main()
{
	Radians<double> radians;
	Degrees<int> degrees;
	PiFactor<double> pifactor;

	while (true)
	{
		Vector<2, float> vector;
		Degrees<float> angle;
		float length;
		prompt("angle:   ", angle.native());
		prompt("length:  ", length);
		vector = Vector<2, float>(angle, length);
		print(vector.x);
		print(vector.y);
		std::cout << '\n';
	}

	std::cin.ignore();
	std::cin.ignore();
}