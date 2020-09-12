#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include "chase-pointers.h"
#include "uniform-int-distribution.h"
using namespace std;

void** create_random_chain(size_t size) {
	size_t len = size / sizeof(void*);
	void** memory = new void*[len];

	UniformIntDistribution uniform;

	size_t* indices = new size_t[len];
	for (size_t i = 0; i < len; ++i) {
		indices[i] = i;
	}
	for (size_t i = 0; i < len - 1; ++i) {
		size_t j = i + uniform.draw(len - i);
		if (i != j) {
			swap(indices[i], indices[j]);
		}
	}

	for (size_t i = 1; i < len; ++i) {
		memory[indices[i - 1]] = (void*)&memory[indices[i]];
	}
	memory[indices[len - 1]] = (void*)&memory[indices[0]];
	delete[] indices;
	return memory;
}

unsigned int log2(size_t val) {
	unsigned int count = 0;
	while (val >>= 1) {
		++count;
	}
	return count;
}

#ifndef MIN_SIZE
#define MIN_SIZE 1024*1024
#endif
#ifndef MAX_SIZE
#define MAX_SIZE 1024*1024
#endif
#ifndef GRANULARITY
#define GRANULARITY (1u)
#endif

int main() {
	printf("   memsize  time in ns\n");
	for (size_t memsize = MIN_SIZE; memsize <= MAX_SIZE;
		memsize += (size_t{ 1 } <<
		(max(GRANULARITY, log2(memsize)) - GRANULARITY))) {
		void** memory = create_random_chain(memsize);
		size_t count = 500000;
		double t = chase_pointers(memory, count);
		delete[] memory;
		double ns = t * 1000000000 / count;
		printf(" %9zu  %10.5lf\n", memsize, ns); cout.flush();
	}
}
