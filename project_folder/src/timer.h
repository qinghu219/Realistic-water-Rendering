#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <mach/clock.h>
#include <mach/mach.h>
class cTimer {
  private:
	mach_timespec_t process_start, frame_start, current;
    clock_serv_t cclock;
  protected:
  public:
	cTimer();
	~cTimer();
	double elapsed(bool frame);
};

#endif