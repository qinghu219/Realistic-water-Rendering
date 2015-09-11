#include "vec.h"

vec3::vec3():x(0.f), y(0.f), z(0.f) {}
vec3::vec3(float x, float y, float z): x(x), y(y), z(z) {}

float vec3::operator*(vec3 const &v)
{
	return this->x*v.x+this->y*v.y+this->z*v.z;
}

vec3 vec3::cross(vec3 const &v)
{
	return vec3(this->y*v.z-this->z*v.y, this->z*v.x-this->x*v.z, this->x*v.y-this->y*v.z);
}

vec3 vec3::operator+(vec3 const &v)
{
	return vec3(this->x+v.x, this->y+v.y, this->z+v.z);	
}
vec3 vec3::operator-(vec3 const &v)
{
	return vec3(this->x-v.x, this->y-v.y, this->z-v.z);	
}
vec3 vec3::operator*(float const f)
{
	return vec3(this->x*f,this->y*f,this->z*f);
}
vec3& vec3::operator=(vec3 const &v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	return *this;
}
float vec3::length()
{
	return sqrt(this->x*this->x+this->y*this->y+this->z*this->z);
}
vec3 vec3::norm()
{
	float len = this->length();
	return vec3(this->x/len,this->y/len,this->z/len);		
}


vec2::vec2():x(0.f), y(0.f) {}
vec2::vec2(float x, float y): x(x), y(y) {}

float vec2::operator*(vec2 const &v)
{
	return this->x*v.x+this->y*v.y;
}

vec2 vec2::operator+(vec2 const &v)
{
	return vec2(this->x+v.x, this->y+v.y);	
}
vec2 vec2::operator-(vec2 const &v)
{
	return vec2(this->x-v.x, this->y-v.y);
}
vec2 vec2::operator*(float const f)
{
	return vec2(this->x*f,this->y*f);
}
vec2& vec2::operator=(vec2 const &v)
{
	this->x = v.x;
	this->y = v.y;
	return *this;
}
float vec2::length()
{
	return sqrt(this->x*this->x+this->y*this->y);
}
vec2 vec2::norm()
{
	float len = this->length();
	return vec2(this->x/len,this->y/len);		
}


