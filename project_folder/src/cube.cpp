#include "cube.h"
#include <string>
void cube::init()
{
	loadCubeShaders();
	GLuint cubeTexture;
	setupCubeTexture(cubeTexture);
	renderCubeTexture();
}
char* cube::file2string(const char *path)
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
void cube::setupCubeTexture(GLuint& curTexture) 
{
	std::string name = "sky";
    std::string type = "tga";
    
	SDL_Surface *x1 = IMG_Load(("sky_img/"+name+"/"+name+"_right."+type).c_str()); SDL_Surface *x2 = IMG_Load(("sky_img/"+name+"/"+name+"_left."+type).c_str());
	SDL_Surface *y1 = IMG_Load(("sky_img/"+name+"/"+name+"_top."+type).c_str()); SDL_Surface *y2 = IMG_Load(("sky_img/"+name+"/"+name+"_down."+type).c_str());
	SDL_Surface *z1 = IMG_Load(("sky_img/"+name+"/"+name+"_front."+type).c_str()); SDL_Surface *z2 = IMG_Load(("sky_img/"+name+"/"+name+"_back."+type).c_str());
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &curTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, curTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, x1->w, x1->h, 0,  GL_BGRA, GL_UNSIGNED_BYTE, x1->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, x2->w, x2->h, 0,  GL_BGRA, GL_UNSIGNED_BYTE, x2->pixels);
    	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, y1->w, y1->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, y1->pixels);
    	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, y2->w, y2->h, 0,  GL_BGRA, GL_UNSIGNED_BYTE, y2->pixels);
    	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, z1->w, z1->h, 0,  GL_BGRA, GL_UNSIGNED_BYTE, z1->pixels);
    	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, z2->w, z2->h, 0,  GL_BGRA, GL_UNSIGNED_BYTE, z2->pixels);
	
	SDL_FreeSurface(x1);  
	SDL_FreeSurface(x2);
	SDL_FreeSurface(y1);  
	SDL_FreeSurface(y2);
	SDL_FreeSurface(z1);  
	SDL_FreeSurface(z2);


}

void cube::loadCubeShaders()
{
	char *vsSource = file2string("shaders/sky.vert");
	char *fsSource = file2string("shaders/sky.frag");	

	cube_vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(cube_vs, 1, &vsSource, NULL);
	glCompileShader(cube_vs);
	GLint a;
	glGetShaderiv(cube_vs, GL_COMPILE_STATUS, &a);
	printf("VS SHADER STATUS: %d\n", a);

	cube_fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(cube_fs, 1, &fsSource, NULL);
	glCompileShader(cube_fs);
	glGetShaderiv(cube_fs, GL_COMPILE_STATUS, &a);
	printf("FS SHADER STATUS: %d\n", a);

	free(vsSource);
	free(fsSource);

	cubeProgram = glCreateProgram();
	glAttachShader(cubeProgram, cube_vs);
	glAttachShader(cubeProgram, cube_fs);
	glLinkProgram(cubeProgram);
	GLint b;
	glGetProgramiv(cubeProgram, GL_LINK_STATUS, &a);
	glValidateProgram(cubeProgram);
	glGetProgramiv(cubeProgram, GL_VALIDATE_STATUS, &b);
	printf("Cube Program STATUS: link %d validate %d\n", a, b);
	
}
void cube::renderCubeTexture()
{
	GLfloat cube_vertices[8][3] = {
	  {-1.0,  1.0,  1.0},
	  {-1.0, -1.0,  1.0},
	  { 1.0, -1.0,  1.0},
	  { 1.0,  1.0,  1.0},
	  {-1.0,  1.0, -1.0},
	  {-1.0, -1.0, -1.0},
	  {1.0, -1.0, -1.0},
	  {1.0,  1.0, -1.0}
	};
	GLushort cube_indices[] = {
	  0, 1, 2, 3,
	  3, 2, 6, 7,
	  7, 6, 5, 4,
	  4, 5, 1, 0,
	  0, 3, 7, 4,
	  1, 2, 6, 5,
	};
	
	glGenBuffers(1, &cube_arrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cube_arrayBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &cube_eleBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_eleBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

}

void cube::cubeRender()
{
	glUseProgram(cubeProgram);
	glBindBuffer(GL_ARRAY_BUFFER, cube_arrayBuffer);
	GLint vertex_position = glGetAttribLocation(cubeProgram, "vp");
        glVertexAttribPointer(vertex_position, 3, GL_FLOAT,
                                   GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(vertex_position);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_eleBuffer);
	glDrawElements(GL_QUADS, 4*6, GL_UNSIGNED_SHORT, NULL);
	
}
