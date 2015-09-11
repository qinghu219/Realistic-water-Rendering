#include "c_num.h"

int c_num::add = 0;
int c_num::mul = 0;

c_num::c_num() : a(0.f), b(0.f) {}
c_num::c_num(float a, float b) : a(a), b(b) {}
c_num c_num::conjugate() {return c_num(this->a, -this->b);}
c_num c_num::operator+(c_num const &c) const 
{
	++c_num::add;
	return c_num(this->a + c.a, this->b + c.b);
}
c_num c_num::operator-(c_num const &c) const 
{
	++c_num::add;
	return c_num(this->a - c.a, this->b - c.b);
}
c_num c_num::operator*(c_num const &c) const 
{
	++c_num::mul;
	return c_num(this->a*c.a-this->b*c.b, this->a*c.b+this->b*c.a);
}
c_num c_num::operator*(float const c) const 
{
	return c_num(this->a*c, this->b*c);
}
c_num& c_num::operator=(c_num const &c) 
{
	this->a = c.a; this->b = c.b;
	return *this;
}
void c_num::reset()
{
	c_num::add = 0;
	c_num::mul = 0;
}
