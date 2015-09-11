#include "ocean.h"
#include <iostream>
char* ocean::file2string(const char *path)
{
	FILE *fd;
	long len, r;
	char *str;
	if (!(fd = fopen(path, "r"))) {
		fprintf(stderr, "Can't open file\n");
		return NULL;
	} 
	fseek(fd, 0, SEEK_END);
	len = ftell(fd);
	printf("File '%s' is %ld long\n", path, len);
	
	fseek(fd, 0, SEEK_SET);
	if (!(str = (char*)malloc(len*sizeof(char)))) {
		fprintf(stderr, "Can't malloc space\n");
		return NULL;
	}
	
	r = fread(str, sizeof(char), len, fd);
	str[r-1] = '\0';
	fclose(fd);
	return str;
}

ocean::ocean(int const N, float const A, vec2 const w, float const length):
	g(9.81), N(N), A(A), w(w), length(length), v(0), ind(0), fftSolver(0)
{
	h_t = new c_num[N*N]; h_t_sx = new c_num[N*N]; h_t_sz = new c_num[N*N]; h_t_dx = new c_num[N*N]; h_t_dz = new c_num[N*N];
	v = new v_ocean[(N+1)*(N+1)];
	ind = new unsigned int[(N+1)*(N+1)*10];
	fftSolver = new fft(N);
	c_num h0, _h0;
	for (int m = 0; m < N+1; ++m)
		for (int n = 0; n < N+1; ++n) {
			int now = n+m*(N+1);
			h0 = H0(n,m);
			_h0 = H0(-n,-m).conjugate();
			v[now].h0a = h0.a;
			v[now].h0b = h0.b;
			v[now]._h0a = _h0.a;
			v[now]._h0b = _h0.b;
			v[now].ox = (n-N/2.f)*length/N;
			v[now].x = (n-N/2.f)*length/N;
			v[now].oy = 0.f;
			v[now].y = 0.f;
			v[now].z = (m-N/2.f)*length/N;
			v[now].oz = (m-N/2.f)*length/N;
			v[now].normal_x = 0.f;
			v[now].normal_y = 1.f;
			v[now].normal_z = 0.f;
		} 	
	flag = 0;
	for (int m = 0; m < N; ++m)
		for (int n = 0; n < N; ++n) {
			int now = n+m*(N+1);
			ind[flag++] = now;
			ind[flag++] = now + N + 1;
			ind[flag++] = now + N + 2;
			ind[flag++] = now;
			ind[flag++] = now + N + 2;
			ind[flag++] = now + 1;		
		}


	loadOceanShaders();
	vertex_position = glGetAttribLocation(program, "vp");
	normal_position = glGetAttribLocation(program, "np");
	texture_position = glGetAttribLocation(program, "tp");
	light_position	= glGetUniformLocation(program, "lp");
	view_position = glGetUniformLocation(program, "VM");
	model_position = glGetUniformLocation(program, "MM");
	inv_vm_position = glGetUniformLocation(program, "TVM");
	eye_position = glGetUniformLocation(program, "eye_position");

	glGenBuffers(1, &arrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(v_ocean)*(N+1)*(N+1), v, GL_STATIC_DRAW);

	glGenBuffers(1, &eleBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, flag*sizeof(unsigned int), ind, GL_STATIC_DRAW);
}
void ocean::set_A(float A)
{
	c_num h0, _h0;
	this->A = A;
	for (int m = 0; m < N+1; ++m)
	for (int n = 0; n < N+1; ++n) {
		int now = n+m*(N+1);
		h0 = H0(n,m);
		_h0 = H0(-n,-m).conjugate();
		v[now].h0a = h0.a;
		v[now].h0b = h0.b;
		v[now]._h0a = _h0.a;
		v[now]._h0b = _h0.b;
		v[now].ox = (n-N/2.f)*length/N;
		v[now].x = (n-N/2.f)*length/N;
		v[now].oy = 0.f;
		v[now].y = 0.f;
		v[now].z = (m-N/2.f)*length/N;
		v[now].oz = (m-N/2.f)*length/N;
		v[now].normal_x = 0.f;
		v[now].normal_y = 1.f;
		v[now].normal_z = 0.f;
	} 	

}
c_num ocean::gaussian()
{
	float a, b, c;
	a = 2.f*(float)rand()/RAND_MAX - 1.f;
	b = 2.f*(float)rand()/RAND_MAX - 1.f;
	c = a*a+b*b;
	while (c >= 1.f) {
		a = 2.f*(float)rand()/RAND_MAX - 1.f;
		b = 2.f*(float)rand()/RAND_MAX - 1.f;
		c = a*a+b*b;
	}
	c = sqrt((log(c)*-2.f)/c);
	return c_num(a*c,b*c);	
}
c_num ocean::H0(int n, int m)
{
	c_num r = gaussian();
	c_num ret = r*sqrt(P(n,m)*0.5f);
	return ret;	
}

c_num ocean::H(int n, int m, float t)
{
	int now = n+m*(N+1);
	c_num h0(v[now].h0a, v[now].h0b);
	c_num _h0(v[now]._h0a, v[now]._h0b);	
	float a = M_PI / 100.f;
	float kx = M_PI*(2.*n-N)/length;
	float kz = M_PI*(2.*m-N)/length;
	float dis = floor(sqrt(g*sqrt(kx*kx+kz*kz))/a)*a;
	float c = cos(dis*t);
	float s = sin(dis*t);	
	c_num p(c, s);
	c_num q(c, -s);
	return h0*p+_h0*q;			
}
void ocean::calc_y(float t)
{
	float lambda = 0.0;
	float delta = 1e-6;
	for (int m = 0; m < N; ++m) {
		float k_z = (2.*M_PI*m-M_PI*N)/length;
		for (int n = 0; n < N; ++n) {
			float k_x = (2.*M_PI*n-M_PI*N)/length;
			float s = sqrt(k_x*k_x+k_z*k_z);
			int now = m*N+n;
			h_t[now] = H(n, m, t);
			h_t_sx[now] = c_num(0,k_x)*h_t[now];
			h_t_sz[now] = c_num(0,k_z)*h_t[now];
			h_t_dx[now] = c_num(0.f,0.f);
			h_t_dz[now] = c_num(0.f,0.f);
			if (s >= delta) {
				h_t_dx[now] = c_num(0.f,-k_x/s) * h_t[now];
				h_t_dz[now] = c_num(0.f,-k_z/s) * h_t[now];
			}
		}
	}	
	for (int m = 0; m < N; ++m) {
		fftSolver->process(h_t, h_t, 1, m*N);
		fftSolver->process(h_t_sx,h_t_sx,1,m*N);
		fftSolver->process(h_t_sz,h_t_sz,1,m*N);
		fftSolver->process(h_t_dx,h_t_dx,1,m*N);
		fftSolver->process(h_t_dz,h_t_dz,1,m*N);
	}
	for (int n = 0; n < N; ++n) {
		fftSolver->process(h_t,h_t,N,n);
		fftSolver->process(h_t_sx,h_t_sx,N,n);
		fftSolver->process(h_t_sz,h_t_sz,N,n);
		fftSolver->process(h_t_dx,h_t_dx,N,n);
		fftSolver->process(h_t_dx,h_t_dx,N,n);
	}

	float ones[] = {1.f,-1.f};
	vec3 n;
	for (int m = 0; m < N; ++m)
		for (int n = 0; n < N; ++n) {
			int now = n+m*(N+1);
			int now_h = n+m*N;
			int cur = ones[(n+m)&1];
			h_t[now_h] = h_t[now_h]*cur;
			v[now].y = h_t[now_h].a; 
			h_t_dx[now_h] = h_t_dx[now_h] * cur;
			h_t_dz[now_h] = h_t_dz[now_h] * cur;	
			v[now].x = v[now].ox + lambda*h_t_dx[now_h].a;
			v[now].z = v[now].oz + lambda*h_t_dz[now_h].a;
			h_t_sx[now_h] = h_t_sx[now_h] * cur;
			h_t_sz[now_h] = h_t_sz[now_h] * cur;
			vec3 curNormal = vec3(-h_t_sx[now_h].a,1.f,-h_t_sz[now_h].a).norm();
			v[now].normal_x = curNormal.x; v[now].normal_y = curNormal.y; v[now].normal_z = curNormal.z;

			if (m == 0 && n == 0) {
				v[now+N+(N+1)*N].y = h_t[now_h].a;
				v[now+N+(N+1)*N].x = v[now+N+(N+1)*N].ox + lambda*h_t_dx[now_h].a;
				v[now+N+(N+1)*N].z = v[now+N+(N+1)*N].oz + lambda*h_t_dz[now_h].a;
				v[now+N+(N+1)*N].normal_x = v[now].normal_x;
				v[now+N+(N+1)*N].normal_y = v[now].normal_y;
				v[now+N+(N+1)*N].normal_z = v[now].normal_z;	
			}
			if (m == 0) {
				v[now+(N+1)*N].y = h_t[now_h].a;
				v[now+(N+1)*N].x = v[now+(N+1)*N].ox+lambda*h_t_dx[now_h].a;
				v[now+(N+1)*N].z = v[now+(N+1)*N].oz+lambda*h_t_dz[now_h].a;
				v[now+(N+1)*N].normal_x = v[now].normal_x;
				v[now+(N+1)*N].normal_y = v[now].normal_y;
				v[now+(N+1)*N].normal_z = v[now].normal_z;
				
			}
			if (n == 0) {
				v[now+N].y = h_t[now_h].a;
				v[now+N].x = v[now+N].ox+lambda*h_t_dx[now_h].a;
				v[now+N].z = v[now+N].oz+lambda*h_t_dz[now_h].a;
				v[now+N].normal_x = v[now].normal_x;
				v[now+N].normal_y = v[now].normal_y;
				v[now+N].normal_z = v[now].normal_z;
			}
		}
}
float ocean::P(int n, int m)
{
	vec2 k(M_PI*(2*n-N)/length, M_PI*(2*m-N)/length);
	float k_l = k.length();
	float V = w.length();
	float kw = k.norm() * w.norm();
	float L = V*V/g;
	float delta = 1e-6;
	float ret;
	if (k_l < delta) 	
		ret = 0.0;
	else
		ret = A * exp(-1.f/(k_l * k_l)/L/L)/(k_l*k_l*k_l*k_l) * (kw) * (kw) * exp(-k_l*k_l*(L*L*delta));
	return ret;
}

void ocean::render(float t)
{
	calc_y(t);
	
	glUseProgram(program);
//	glUniform3f(light_position, 180.f, 10.f, -220.f);
    glUniform3f(light_position, 2000, 1600, 2000);
	glBindBuffer(GL_ARRAY_BUFFER, this->arrayBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v_ocean)*(N+1)*(N+1), v);
	
	glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, sizeof(v_ocean), 0);
	glEnableVertexAttribArray(vertex_position);
	glVertexAttribPointer(normal_position, 3, GL_FLOAT, GL_FALSE, sizeof(v_ocean), (char *)NULL + 12);
	glEnableVertexAttribArray(normal_position);
	glVertexAttribPointer(texture_position, 3, GL_FLOAT, GL_FALSE, sizeof(v_ocean), (char *)NULL + 24);	
	glEnableVertexAttribArray(texture_position);


	glUniformMatrix4fv(view_position, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(model_position, 1, GL_FALSE, glm::value_ptr(model));
	tvm = glm::inverse(glm::transpose(view*model));
	glUniformMatrix4fv(inv_vm_position, 1, GL_FALSE, glm::value_ptr(tvm));
	tmodel = glm::inverse(glm::transpose(model));
	glUniformMatrix4fv(inv_m_position, 1, GL_FALSE, glm::value_ptr(tmodel));
	glUniform3fv(eye_position, 1, glm::value_ptr(ep));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eleBuffer);

	for (int j = -10; j < 10; ++j)
		for (int i = -10; i < 10; ++i) {
			model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f,0.5f,0.5f));
			model = glm::translate(model, glm::vec3(length*i,0,length*j));
//			model = glm::translate(model, glm::vec3(0,-5,0));

			glUniformMatrix4fv(model_position, 1, GL_FALSE, glm::value_ptr(model));
			tvm = glm::inverse(glm::transpose(view*model));
			glUniformMatrix4fv(inv_vm_position, 1, GL_FALSE, glm::value_ptr(tvm));
			glDrawElements(GL_TRIANGLES, flag, GL_UNSIGNED_INT, 0);	
		}
}
void ocean::loadOceanShaders()
{
	char *vsSource = file2string("shaders/ocean_sky.vert");
	char *fsSource = file2string("shaders/ocean_sky.frag");	

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsSource, NULL);
	glCompileShader(vs);
	GLint a;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &a);
	printf("VS SHADER STATUS: %d\n", a);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsSource, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &a);
	printf("FS SHADER STATUS: %d\n", a);

	free(vsSource);
	free(fsSource);

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	GLint b;
	glGetProgramiv(program, GL_LINK_STATUS, &a);
	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &b);
	printf("Ocean Program STATUS: link %d validate %d\n", a, b);
}


