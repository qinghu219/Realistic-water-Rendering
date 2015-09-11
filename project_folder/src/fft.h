#ifndef fft_H
#define fft_H

#include <math.h>
#include "c_num.h"

class fft {
  public:
	int N, current;
	int *rev;
	c_num **w;
	c_num *c[2];

	fft(int N);
	int reverse(int i);
	void process(c_num* a, c_num* b, int x, int y);
	c_num ww(int x, int N);
};

#endif