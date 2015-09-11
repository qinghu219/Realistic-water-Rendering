#ifndef c_num_H
#define c_num_H
class c_num
{
	public:
		float a, b;
		static int add, mul;
		c_num conjugate();
		c_num();
		c_num(float a, float b);	
		c_num operator+(c_num const &c) const;
		c_num operator-(c_num const &c) const;
		c_num operator*(c_num const &c) const;
		c_num operator*(float const c) const;
		c_num& operator=(c_num const &c);
		static void reset();
		
};
#endif