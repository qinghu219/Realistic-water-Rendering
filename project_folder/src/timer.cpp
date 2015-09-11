#include "timer.h"


cTimer::cTimer() {
    
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &process_start);
	frame_start = process_start;
}

cTimer::~cTimer() {
	
}

double cTimer::elapsed(bool frame) {
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &current);

	double elapsed = frame ? (current.tv_sec + current.tv_nsec / 1000000000.0 -   frame_start.tv_sec -   frame_start.tv_nsec / 1000000000.0) :
				 (current.tv_sec + current.tv_nsec / 1000000000.0 - process_start.tv_sec - process_start.tv_nsec / 1000000000.0);
	frame_start = current;
	return elapsed;
}