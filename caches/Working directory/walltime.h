#ifndef WALLTIME_H
#define WALLTIME_H

#include <chrono>
using namespace std;

template<typename T>
class WallTime {
public:
	WallTime() : t0(chrono::high_resolution_clock::now()) {
	}
	/* return real time in seconds using T */
	T elapsed() const {
		using namespace std::chrono;
		auto time_spent = high_resolution_clock::now() - t0;
		return duration<T, seconds::period>(time_spent).count();
	}
private:
	chrono::high_resolution_clock::time_point t0;
};

#endif
