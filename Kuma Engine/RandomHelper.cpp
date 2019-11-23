#include "Globals.h"
#include "RandomHelper.h"
#include "pcg-cpp-0.98/include/pcg_random.hpp"
#include <random>

float RandomFloatGenerator()
{
	// Seed with a real random value, if available
	pcg_extras::seed_seq_from<std::random_device> seed_source;

	// Make a random number engine
	pcg32 rng(seed_source);

	// Choose a random mean between 1 and 6
	std::uniform_int_distribution<int> uniform_dist(0, 10);
	float mean = uniform_dist(rng);
	float result = mean / 10;
	LOG("%f.2", result);

	return result;

}

int RandomintGenerator(int min, int max)
{
	// Seed with a real random value, if available
	pcg_extras::seed_seq_from<std::random_device> seed_source;

	// Make a random number engine
	pcg32 rng(seed_source);

	// Choose a random mean between 1 and 6
	std::uniform_int_distribution<int> uniform_dist(min, max);
	int mean = uniform_dist(rng);

	LOG("%i", mean);

	return mean;
}

UINT32 GetRandomID()
{
	pcg_extras::seed_seq_from<std::random_device> seed;
	pcg32 rng(seed);
	std::uniform_int_distribution<UINT32> uniform_d(1, ULONG_MAX);

	return uniform_d(rng);
}

UID GetRandomUID()
{
	pcg_extras::seed_seq_from<std::random_device> seed;
	pcg32 rng(seed);

	std::uniform_int_distribution<UID> uniform_d(1, ULONG_MAX);

	return uniform_d(rng);
}

void EstablishRand()
{

}
