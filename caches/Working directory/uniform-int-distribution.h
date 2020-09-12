#ifndef UNIFORM_INT_DISTRIBUTION_H
#define UNIFORM_INT_DISTRIBUTION_H

#include <random>
using namespace std;

class UniformIntDistribution {
public:
	UniformIntDistribution() : engine(random_device()()) {}
	/* return number in the range of [0..upper_limit) */
	unsigned int draw(unsigned int upper_limit) {
		return uniform_int_distribution<unsigned int>
			(0, upper_limit - 1)(engine);
	}
private:
	mt19937 engine;
};

#endif
