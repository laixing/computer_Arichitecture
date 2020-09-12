#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <unistd.h>
#include <sys/times.h>
#include "chase-pointers.h"
using namespace std;

static unsigned int log2(unsigned int val) {
	unsigned int count = 0;
	while (val >>= 1) {
		++count;
	}
	return count;
}

static unsigned int bit_reverse(unsigned int val, unsigned int bits) {
	unsigned int result = 0;
	while (bits > 0) {
		result = (result << 1) | (val & 1);
		val >>= 1;
		--bits;
	}
	return result;
}

static void gen_bit_reversal_permutation(unsigned int* seq,
	unsigned int bits, unsigned int count) {
	/* generate a bit-reversal permutation for integers from 0 to (2^bits)-1 */
	unsigned int maxval = 1 << bits;
	for (unsigned int val = 0; val < maxval; ++val) {
		seq[val] = bit_reverse(val, bits);
	}
	/* cut sequence short if count is not a power of 2, i.e. count < 2^bits */
	unsigned int current = maxval;
	unsigned int index = 0;
	while (current > count) {
		while (seq[index] < count) ++index;
		--current; seq[index] = seq[current];
	}
}

void** create_linear_chain(size_t size, size_t stride) {
	size_t len = size / sizeof(void*);
	void** memory = new void*[len];
	unsigned int runs = stride / sizeof(void*);
	unsigned int bits = log2(runs);
	if ((size_t)(1 << bits) != runs) ++bits;
	unsigned int* offset = new unsigned int[1 << bits];
	gen_bit_reversal_permutation(offset, bits, runs);

	/* generate the actual pointer chain */
	//unsigned int run = 0;
	void** last = nullptr;
	for (unsigned int run = 0; run < runs; ++run) {
		char* next = (char*)memory + offset[run] * sizeof(void*);
		if (last) {
			*last = (void*)next;
		}
		last = (void**)next;
		for (;;) {
			char* next = (char*)last + stride;
			if (next >= (char*)memory + size) break;
			*last = (void*)next; last = (void**)next;
		}
	}
	*last = (void*)memory; /* close the cycle */
	return memory;
}



#ifndef MIN_STRIDE
#define MIN_STRIDE (sizeof(void*))
#endif
#ifndef MAX_STRIDE
#define MAX_STRIDE 1200
#endif

int main() {
	printf("   stride  time in ns\n");
	for (size_t stride = MIN_STRIDE; stride <= MAX_STRIDE;
		stride += sizeof(void*)) {
		size_t memsize = min(size_t{ 1 } << 26,
			stride * 1024 * sizeof(void*));
		void** memory = create_linear_chain(memsize, stride);
		size_t count = 500000;
		double t = chase_pointers(memory, count);
		delete[] memory;
		double ns = t * 1000000000 / count;
		printf(" %8zu  %10.5lf\n", stride, ns); cout.flush();
	}
}
