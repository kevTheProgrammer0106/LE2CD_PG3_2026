#include <stdio.h>

using namespace std;

template<typename Type>
Type Min(Type a, Type b)
{
	return static_cast< Type >( a < b ? a : b );
}

int main()
{
	int intValue1 = 34;
	int intValue2 = 54;

	float floatValue1 = 6.5f;
	float floatValue2 = 2.5f;

	double doubleValue1 = 3.14;
	double doubleValue2 = 21.71;

	int a = Min<int>(intValue1, intValue2);
	float b = Min<float>(floatValue1, floatValue2);
	double c = Min<double>(doubleValue1, doubleValue2);

	printf("Min of %d and %d is:", intValue1, intValue2);
	printf(" %d\n", a);

	printf("Min of %f and %f is:", floatValue1, floatValue2);
	printf(" %f\n", b);

	printf("Min of %f and %f is:", doubleValue1, doubleValue2);
	printf(" %f\n", c);

	return 0;
}