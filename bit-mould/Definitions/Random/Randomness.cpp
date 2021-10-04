#include "Randomness.h"

using namespace std;

float Randomness::dist_1_0()
{
	return distribution_1_0(generator);
}

int Randomness::dist_8_0()
{
	return distribution_direction(generator);
}
int Randomness::dist_custom(const int & begin, const int & end)
{
	uniform_int_distribution<int> dist_custom = uniform_int_distribution<int>(begin, end);
	return dist_custom(generator);
}
mt19937_64 Randomness::generator(time(0));
uniform_real_distribution<float> Randomness::distribution_1_0 = uniform_real_distribution<float>(0.0f, 1.0f);
uniform_int_distribution<int> Randomness::distribution_direction = uniform_int_distribution<int>(0, 8);
