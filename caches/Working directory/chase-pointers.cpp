#include "chase-pointers.h"
#include "walltime.h"
#include <iostream>
using namespace std;

volatile void* chase_pointers_global;

double chase_pointers(void** memory, size_t count) {
	WallTime<double> walltime;

	// count times
	void** p = (void**)memory;
	while (count-- > 0) {
		p = (void**)*p;
	}
	auto elapsed = walltime.elapsed();
	chase_pointers_global = *p;
	return elapsed;
}
