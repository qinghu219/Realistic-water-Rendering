#ifndef ocean_H 
#define ocean_H

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "vec.h"
#include "c_num.h"
#include "fft.h"
#include <stdlib.h>
#include <stdio.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
class v_ocean {
	public:
		GLfloat x, y, z;
		GLfloat normal_x, normal_y, normal_z;
		GLfloat h0a, h0b, h0c;
		GLfloat _h0a, _h0b, _h0c;
		GLfloat ox, oy, oz;
	
};
class ocean {
	public:
		float g, A;
		int N;
		vec2 w;
		float length;
		c_num *h_t, *h_t_sx, *h_t_sz, *h_t_dx, *h_t_dz;
		v_ocean *v;
		unsigned int *ind;
		glm::mat4 view, model, tvm, tmodel;
		glm::vec3 ep;
		int flag;
		fft *fftSolver;
		GLuint program, arrayBuffer, eleBuffer, oceanArray, vs, fs;
		GLint vertex_position, normal_position, texture_position, light_position, view_position, model_position, inv_vm_position, inv_m_position, eye_position;
		ocean(int const N, float const A, vec2 const w, float const length); //construct method
		c_num H0(int n, int m); //H0(n,m)
		c_num H(int n, int m, float t);
		void calc_y(float t); //calculate y at every point
		float P(int n, int m);//phillips spec
		void loadOceanShaders(); //load ocean shader for vertex and fragment
		void render(float t); //render at time t
		c_num gaussian();
		char* file2string(const char *path);
		void set_A(float A);


};

#endif
