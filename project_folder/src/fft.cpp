#include "fft.h"

fft::fft(int N): N(N), w(0), rev(0) {
	c[0] = 0;
	c[1] = 0;
	rev = new int[N];
	for (int i = 0; i < N; ++i)
		rev[i] = reverse(i);
	w = new c_num*[(int)(log(N)/log(2))];
	int power = 1;
	for (int i = 0; i < (int)(log(N)/log(2)); ++i) {
		w[i] = new c_num[power];
		for (int j = 0; j < power; ++j)
			w[i][j] = ww(j, power << 1);
		power = (power << 1);
	}	
	c[0] = new c_num[N];
	c[1] = new c_num[N];
	current = 0;
}

int fft::reverse(int i)
{
	int now = 0;
	for (int j = 0; j < log(N)/log(2); ++j) {
		now = (i & 1) + (now << 1);
		i = (i >> 1);
	}
	return now;
}

c_num fft::ww(int x, int N)
{
	return c_num(cos(x*M_PI*2.0/N), sin(x*M_PI*2.0/N));
}


void fft::process(c_num* a, c_num* b, int x, int y)
{
	int flag = 0;
	int xx = 1;
	int yy = 2;
	int l = (N>>1);
	for (int i = 0; i < N; ++i)
		c[current][i] =	a[rev[i]*x+y];
	for (int i = 1; i <= log(N)/log(2); ++i) {
		current = (current ^ 1);
		for (int j = 0; j < l; ++j) {
			for (int k = 0; k < xx; ++k) 
				c[current][yy*j+k] = c[current^1][yy*j+k]+c[current^1][yy*j+k+xx]*w[flag][k];
			for (int k = xx; k < yy; ++k)
				c[current][k+j*yy] = c[current^1][j*yy+k-xx]-c[current^1][j*yy+k]*w[flag][k-xx];
		}
		xx <<= 1;
		yy <<= 1;
		++flag;
		l >>= 1;
	}
	for (int i = 0; i < N; ++i)
		b[i*x+y] = c[current][i];
}
